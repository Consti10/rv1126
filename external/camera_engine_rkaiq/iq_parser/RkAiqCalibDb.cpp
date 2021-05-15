/*
 *  Copyright (c) 2019 Rockchip Corporation
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
 *
 */


#include "RkAiqCalibDb.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#define FUNC_ENTER LOG1("%s enter", __FUNCTION__);
#define FUNC_EXIT LOG1("%s exit", __FUNCTION__);
#define CALIBDB_PATH_LEN      256
#define CALIBDB_ENV_PATH_STR  "rkaiq_calibdb_path"

namespace RkCam {

map<string, CamCalibDbContext_t*> RkAiqCalibDb::mCalibDbsMap;
XCam::Mutex RkAiqCalibDb::mMutex{false};
static bool calibGetEnviromentPath(const char* variable, char* value)
{
    if (!variable || !value) {
        return false;
    }

    char* valueStr = getenv(variable);
    if (valueStr) {
        strncpy(value, valueStr, CALIBDB_PATH_LEN - 1);
        return true;
    }
    return false;
}

static void getFilePathName(char* iqFile, char *location)
{
    char dir[CALIBDB_PATH_LEN];
    char name[CALIBDB_PATH_LEN];
    char *srcfile = strdup(iqFile);
    char *pstart = strrchr(srcfile, '/');
    char *pend = strrchr(srcfile, '.');
    *pend = '\0';
    strcpy(name, pstart + 1);
    if (calibGetEnviromentPath(CALIBDB_ENV_PATH_STR, dir)) {
        sprintf(location, "%s/%s.bin", dir, name);
    } else {
        sprintf(location, "%s.bin", srcfile);
    }
    free(srcfile);
    LOGD("calibdb file is %s", location);
}

static bool isDataBinExist(char* iqFile) {
    char path[CALIBDB_PATH_LEN];

    getFilePathName(iqFile, path);
    if (0 == access(path, F_OK))
        return true;
    else
        return false;
}

static bool calibSaveToFile(char* iqFile, CamCalibDbContext_t* calib)
{
    int fd;
    bool ret = true;
    char path[CALIBDB_PATH_LEN];

    getFilePathName(iqFile, path);
    fd = open(path, O_CREAT | O_TRUNC | O_RDWR | O_SYNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (fd < 0) {
        ret = false;
        LOGE("open %s failed!", path);
    } else {
        lseek(fd, 0, SEEK_SET);
        if (write(fd, calib, sizeof(CamCalibDbContext_t)) <= 0) {
            LOGE("write %s failed!\n", path);
            remove(path);
            ret = false;
        }
        close(fd);
    }
    return ret;
}

static bool calibReadFromFile(char* iqFile, CamCalibDbContext_t* calib)
{
    int fd;
    bool ret = true;
    char path[CALIBDB_PATH_LEN];

    getFilePathName(iqFile, path);
    fd = open(path, O_RDONLY | O_SYNC);
    if (fd < 0) {
        ret = false;
    } else {
        lseek(fd, 0, SEEK_SET);
        if (read(fd, calib, sizeof(CamCalibDbContext_t)) <= 0) {
            LOGE("read %s failed!\n", path);
            ret = false;
        }
        close(fd);
    }

    return ret;
}

CamCalibDbContext_t* RkAiqCalibDb::createCalibDb(char* iqFile)
{
    map<string, CamCalibDbContext_t*>::iterator it;

    string str(iqFile);

    it = mCalibDbsMap.find(str);
    if (it != mCalibDbsMap.end()) {
        LOGD("use cached calibdb for %s!", iqFile);
        return it->second;
    } else {
        CamCalibDbContext_t *pCalibDb = new CamCalibDbContext_t();
        if (pCalibDb) {
            if (0 == access(iqFile, F_OK)) {
                RkAiqCalibParser  parser(pCalibDb);
                mMutex.lock();
                bool ret = parser.doParse(iqFile);
                mMutex.unlock();
                if (ret) {
                    uint32_t magicCode = calib_check_calc_checksum();
                    if (magicCode != pCalibDb->header.magic_code) {
                        LOGE("magic code is not matched! calculated:%u, readed:%u", magicCode, pCalibDb->header.magic_code);
                    }else {
                        mCalibDbsMap[str] = pCalibDb;
                        LOGD("create calibdb from %s success.", iqFile);
                        return pCalibDb;
                    }
                } else {
                    LOGE("parse %s failed.", iqFile);
                }
            } else if(isDataBinExist(iqFile)) {
                if (calibReadFromFile(iqFile, pCalibDb)) {
                    uint32_t magicCode = calib_check_calc_checksum();
                    if (magicCode != pCalibDb->header.magic_code) {
                        LOGE("magic code is not matched! calculated:%u, readed:%u", magicCode, pCalibDb->header.magic_code);
                    }else {
                        mCalibDbsMap[str] = pCalibDb;
                        LOGD("get calibdb from bin success.");
                        return pCalibDb;
                    }
                } else {
                    LOGE("get calibdb from bin failed.");
                }
            } else {
                LOGE("calibdb %s and bin are all not exist!", iqFile);
            }
        } else {
            LOGE("alloc calibdb memory failed.");
        }
    }
    return NULL;
}

bool RkAiqCalibDb::generateCalibDb(char* iqFileRef, char* iqFileOutput, CamCalibDbContext_t* pCalibDb)
{
    //map<string, CamCalibDbContext_t*>::iterator it;

    string str(iqFileRef);

    //it = mCalibDbsMap.find(str);
    if (pCalibDb) {
        RkAiqCalibParser  parser(pCalibDb);
        if (parser.doGenerate(iqFileRef, iqFileOutput)) {
            mCalibDbsMap[str] = pCalibDb;
            LOGD("generate calibdb from %s to %s success.", iqFileRef, iqFileOutput);
            return true;
        }
        else {
            LOGE("generate %s to %s failed.", iqFileRef, iqFileOutput);
        }
    }
    else {
        LOGE("alloc generate memory failed.");
    }
    return false;
}

void RkAiqCalibDb::releaseCalibDb()
{
    std::map<string, CamCalibDbContext_t*>::iterator it;
    for (it = mCalibDbsMap.begin(); it != mCalibDbsMap.end(); it++) {
        CamCalibDbContext_t *pCalibDb = it->second;
        if(pCalibDb) {
            LOGI("releaseCalibDb!");
            delete pCalibDb;
        }
    }
    mCalibDbsMap.clear();
}

CamCalibDbContext_t* RkAiqCalibDb::getCalibDb(char* iqFile)
{
    std::map<string, CamCalibDbContext_t*>::iterator it;

    std::string str(iqFile);

    it = mCalibDbsMap.find(str);
    if (it != mCalibDbsMap.end()) {
        return it->second;
    } else {
        LOGE("calibDb not found!");
        return NULL;
    }
}

void RkAiqCalibDb::createCalibDbBinFromXml(char* iqFile)
{
    CamCalibDbContext_t *pCalibDb = new CamCalibDbContext_t();
    if (pCalibDb) {
        if (0 == access(iqFile, F_OK)) {
            RkAiqCalibParser  parser(pCalibDb);
            if (parser.doParse(iqFile)) {
                uint32_t magicCode = calib_check_calc_checksum();
                if (magicCode != pCalibDb->header.magic_code) {
                    LOGE("magic code is not matched! calculated:%u, readed:%u", magicCode, pCalibDb->header.magic_code);
                }else {
                    LOGI("create calibdb from %s success, magic code %u.", iqFile, magicCode);
                    if (calibSaveToFile(iqFile, pCalibDb))
                        LOGD("save to bin success.");
                    else
                        LOGE("save to bin failed.");
                }
            } else {
                LOGE("parse %s failed.", iqFile);
            }
        } else {
            LOGE("%s is not found!", iqFile);
        }
        delete pCalibDb;
    }
}

}; //namespace RkCam


