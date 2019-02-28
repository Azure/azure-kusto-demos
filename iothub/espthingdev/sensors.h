// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef __SENSORS_H
#define __SENSORS_H


#ifdef __cplusplus
extern "C" {
#endif

void initSensors(void);
void getTempAndHum(float* Temperature, float* Humidity);
void getDistance(long* Distance);

#ifdef __cplusplus
}
#endif


#endif//__SENSORS_H

