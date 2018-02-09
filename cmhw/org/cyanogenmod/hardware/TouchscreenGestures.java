/*
 * Copyright (C) 2016 The CyanogenMod Project
 *               2017-2018 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.cyanogenmod.hardware;

import android.util.Log;

import cyanogenmod.hardware.TouchscreenGesture;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

import org.cyanogenmod.internal.util.FileUtils;

/**
 * Touchscreen gestures API
 *
 * A device may implement several touchscreen gestures for use while
 * the display is turned off, such as drawing alphabets and shapes.
 * These gestures can be interpreted by userspace to activate certain
 * actions and launch certain apps, such as to skip music tracks,
 * to turn on the flashlight, or to launch the camera app.
 *
 * This *should always* be supported by the hardware directly.
 * A lot of recent touch controllers have a firmware option for this.
 *
 * This API provides support for enumerating the gestures
 * supported by the touchscreen.
 */
public class TouchscreenGestures {
    private static final String TAG = "TouchscreenGestures";
    private static final String CMD_LIST_PATH = "/sys/class/sec/tsp/cmd_list";
    private static final String CMD_EXEC_PATH = "/sys/class/sec/tsp/cmd";

    private static final String CMD_SPAY_ENABLE = "spay_enable";

    private static final TouchscreenGesture[] TOUCHSCREEN_GESTURES = {
        new TouchscreenGesture(0, "One finger up swipe home", 0x1c7),
    };

    /**
     * Whether device supports touchscreen gestures
     *
     * @return boolean Supported devices must return always true
     */
    public static boolean isSupported() {
        String line = null;
        BufferedReader reader = null;

        try {
            reader = new BufferedReader(new FileReader(CMD_LIST_PATH), 512);

            line = reader.readLine();
            while (line != null) {
                if (line.trim().equals(CMD_SPAY_ENABLE))
                    return true;
                line = reader.readLine();
            }
        } catch (FileNotFoundException e) {
            Log.w(TAG, "Couldn't open " + CMD_LIST_PATH, e);
        } catch (IOException e) {
            Log.w(TAG, "Couldn't read from file " + CMD_LIST_PATH, e);
        } finally {
            try {
                if (reader != null)
                    reader.close();
            } catch (IOException e) {
            }
        }

        return false;
    }

    /*
     * Get the list of available gestures. A mode has an integer
     * identifier and a string name.
     *
     * It is the responsibility of the upper layers to
     * map the name to a human-readable format or perform translation.
     *
     * @return TouchscreenGesture[] An array of the touchscreen gestures
     *                              available on a device
     */
    public static TouchscreenGesture[] getAvailableGestures() {
        return TOUCHSCREEN_GESTURES;
    }

    /**
     * This method allows to set the activation status of a gesture
     *
     * @param gesture The gesture to be activated
     *        state   The new activation status of the gesture
     *
     * @return boolean Must be false if gesture is not supported
     *         or the operation failed; true in any other case.
     */
    public static boolean setGestureEnabled(
            final TouchscreenGesture gesture, final boolean state) {
        String cmd = CMD_SPAY_ENABLE + "," + (state ? "1" : "0");
        boolean result = FileUtils.writeLine(CMD_EXEC_PATH, cmd);
        if (!result)
            return false;
        return true;
    }
}
