#ifndef SEC_CAMERA_PROPERTIES_H
#define SEC_CAMERA_PROPERTIES_H

/* SEC Camera Properties */

/* Real-Time HDR */
#define LIVE_HDR_LEVEL_RANGE		0x80000001
#define LIVE_HDR_LEVEL			0x80000002

/* Metering Mode */
#define AVAILABLE_METERING_MODE		0x80000003
#define METERING_MODE			0x80000004
/* Options */
#define METERING_MODE_CENTER		0
#define METERING_MODE_SPORT		1
#define METERING_MODE_MATRIX		2
#define METERING_MODE_MANUAL		3

/* Phase-Detection Auto Focus */
#define AVAILABLE_PAF_MODE		0x80000005
#define PAF_MODE			0x80000006
/* Options */
#define PAF_MODE_OFF			0
#define PAF_MODE_ON			1

/* Optical Image Stabilization Operation Mode */
#define AVAILABLE_OIS_OPERATION_MODE	0x80020000
#define OIS_OPERATION_MODE		0x80010000
/* Options */
#define OIS_OPERATION_MODE_PICTURE	0
#define OIS_OPERATION_MODE_VIDEO	1

#endif // SEC_CAMERA_PROPERTIES_H
