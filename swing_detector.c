#include "swing_detector.h"
#include <math.h>

#include "i2c.h"
#include "mpu6050.h"
#include "Time_Out.h"


#define SWING_MIN_PEAK_DPS      18.0f   /* minimum peak angular speed */
#define SWING_MIN_ANGLE_DEG     10.0f   /* minimum total rotation angle */

/* If |omega| ever exceeds this, we consider it a real "swing-ish" motion */
#define SWING_START_THRESHOLD_DPS  10.0f


/* Integrated angles (deg) over the window */
static float s_int_gx = 0.0f;
static float s_int_gy = 0.0f;
static float s_int_gz = 0.0f;

/* Max angular speed in the window */
static float s_max_omega = 0.0f;

/* Did we ever exceed SWING_START_THRESHOLD_DPS in this window? */
static uint8_t s_seenBigOmega = 0;

/* Is a window currently active? */
static uint8_t s_windowActive = 0;

//Swing ring Buffer

#define SWING_EVENT_BUF_SIZE  16

static SwingEvent eventBuf[SWING_EVENT_BUF_SIZE];
static uint8_t    eventHead  = 0;
static uint8_t    eventCount = 0;

MPU6050_Frame_t frame;

const uint32_t WINDOW_MS = 300;      // ~0.3 s swing analysis window
uint8_t  windowActive    = 0;
uint32_t windowStartTs   = 0;
uint32_t lastSampleTs    = 0;


void SwingEventBuffer_Init(void)
{
    eventHead  = 0;
    eventCount = 0;
}

void SwingEventBuffer_Push(uint32_t t_ms, SwingDirection dir)
{
    uint8_t idx = (uint8_t)((eventHead + eventCount) % SWING_EVENT_BUF_SIZE);

    eventBuf[idx].t_ms = t_ms;
    eventBuf[idx].dir  = dir;

    if (eventCount < SWING_EVENT_BUF_SIZE) {
        eventCount++;
    } else {
        /* buffer full → drop oldest, move head forward */
        eventHead = (uint8_t)((eventHead + 1U) % SWING_EVENT_BUF_SIZE);
    }
}

uint8_t SwingEventBuffer_Pop(SwingEvent *out)
{
    if (eventCount == 0) {
        return 0;
    }

    *out = eventBuf[eventHead];
    eventHead = (uint8_t)((eventHead + 1U) % SWING_EVENT_BUF_SIZE);
    eventCount--;

    return 1;
}


void SwingDetector_Init(void)
{
    /* Reset window state */
    s_int_gx       = 0.0f;
    s_int_gy       = 0.0f;
    s_int_gz       = 0.0f;
    s_max_omega    = 0.0f;
    s_seenBigOmega = 0;
    s_windowActive = 0;

    /* Also clear the swing event buffer */
    SwingEventBuffer_Init();
}

/* Start a new integration window */
void SwingDetector_BeginWindow(void)
{
    s_int_gx       = 0.0f;
    s_int_gy       = 0.0f;
    s_int_gz       = 0.0f;
    s_max_omega    = 0.0f;
    s_seenBigOmega = 0;
    s_windowActive = 1;
}

/* Feed one sample into the current window */
void SwingDetector_Update(float gx_dps, float gy_dps, float gz_dps,
                          float ax_lin, float ay_lin, float az_lin,
                          float dt_s)
{
    (void)ax_lin;
    (void)ay_lin;
    (void)az_lin;

    if (!s_windowActive) {
        return;
    }

    if (dt_s <= 0.0f) {
        return;
    }

    /* Integrate gyro angles (deg) over the window */
    s_int_gx += gx_dps * dt_s;
    s_int_gy += gy_dps * dt_s;
    s_int_gz += gz_dps * dt_s;

    /* Magnitude of angular velocity */
    float omega = sqrtf(gx_dps * gx_dps +
                        gy_dps * gy_dps +
                        gz_dps * gz_dps);

    if (omega > s_max_omega) {
        s_max_omega = omega;
    }
    if (omega > SWING_START_THRESHOLD_DPS) {
        s_seenBigOmega = 1;
    }
}

/* End window, decide if a swing happened and which direction */
SwingResult SwingDetector_EndWindow(void)
{
    SwingResult res;
    res.swing_detected = 0;
    res.direction      = SWING_NONE;

    if (!s_windowActive) {
        return res;
    }

    s_windowActive = 0;

    /* Reject if we never saw strong motion */
    if (!s_seenBigOmega || s_max_omega < SWING_MIN_PEAK_DPS) {
        return res;
    }

    /* Total rotation on each axis (absolute angle in deg) */
    float ax = fabsf(s_int_gx);
    float ay = fabsf(s_int_gy);
    float az = fabsf(s_int_gz);

    float maxAngle = ax;    // Pick the dominant axis (largest rotation) and call it axis.
    char  axis     = 'x';

    if (ay > maxAngle) {
        maxAngle = ay;
        axis = 'y';
    }
    if (az > maxAngle) {
        maxAngle = az;
        axis = 'z';
    }

    /* Not enough total rotation → no swing */
    if (maxAngle < SWING_MIN_ANGLE_DEG) {
        return res;
    }

    /* Map axis + sign to direction.
     *
     * I assume:
     *   X axis: LEFT <-> RIGHT
     *   Y axis: UP <-> DOWN
     */

    switch (axis) {
        /* Now: X axis = UP / DOWN */
        case 'x':
            if (s_int_gx > 0.0f) {  // for checking positive or negative angles
                res.direction = SWING_UP;
            } else {
                res.direction = SWING_DOWN;
            }
            res.swing_detected = 1;
            break;

        /* Now: Y axis = LEFT / RIGHT */
        case 'y':
            if (s_int_gy > 0.0f) {
                res.direction = SWING_LEFT_TO_RIGHT;
            } else {
                res.direction = SWING_RIGHT_TO_LEFT;
            }
            res.swing_detected = 1;
            break;

        case 'z':
        default:
            res.swing_detected = 0;
            res.direction      = SWING_NONE;
            break;
        }
    return res;
}

void setup_swing_detector(void){
    Ticks_Init(180000000); // global timer for i2c

    i2c1_init();              // bare-metal I2C init
    MPU6050_DMA_Init();       // DMA extension

    // Configure IMU using your existing driver
    MPU_ConfigTypeDef cfg = {
        .ClockSource      = Internal_8MHz,
        .Gyro_Full_Scale  = FS_SEL_250,
        .Accel_Full_Scale = AFS_SEL_2g,
        .CONFIG_DLPF      = DLPF_94A_98G_Hz,
        .Sleep_Mode_Bit   = 0
    };
    MPU6050_Config(&cfg);

    // Register DMA callback that unpacks samples
    i2c1_dma_setCallback(MPU6050_DMA_DoneHandler);

    // Init swing detector (also clears its internal event buffer)
    SwingDetector_Init();
}

void observe_swing(void){
    // Kick a DMA read (non-blocking)
    MPU6050_DMA_RequestRead();

    // If a new frame is ready, process it
    if (MPU6050_DMA_GetLatest(&frame))
    {
        uint32_t now = frame.timestamp;  // from get_Ticks() in DMA handler

        // Compute dt in seconds between this and previous sample
        float dt_s = 0.0f;
        if (lastSampleTs != 0U) {
            uint32_t dt_ms = now - lastSampleTs;
            dt_s = (float)dt_ms * 0.001f;
        }
        lastSampleTs = now;

        // Convert raw gyro to deg/s (FS = ±250 dps)
        float gx_dps = (float)frame.gx * 250.0f / 32768.0f;
        float gy_dps = (float)frame.gy * 250.0f / 32768.0f;
        float gz_dps = (float)frame.gz * 250.0f / 32768.0f;

        // For now we don’t use linear accel in swing detection
        float ax_lin = 0.0f;
        float ay_lin = 0.0f;
        float az_lin = 0.0f;

        // If no active window, start one at this timestamp
        if (!windowActive) {
            SwingDetector_BeginWindow();
            windowStartTs = now;
            windowActive  = 1;
        }

        // Feed this sample into the current window
        if (dt_s > 0.0f) {
            SwingDetector_Update(gx_dps, gy_dps, gz_dps,
                                    ax_lin, ay_lin, az_lin,
                                    dt_s);
        }

    }
}

SwingResult get_swing_direction(void){

    // If window has reached its duration, classify it and deactivate your window
    SwingResult sr = SwingDetector_EndWindow();
    windowActive = 0;   // next sample will start a new window

    return sr;
}