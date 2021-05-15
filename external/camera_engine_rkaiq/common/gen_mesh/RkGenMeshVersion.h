/*
 *  Copyright (c) 2020 Rockchip Corporation
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

#ifndef __RKGENMESHVERSION_H__
 /*!
  * ==================== GENMESH VERSION HISTORY ====================
  *
  * v1.0.0
  *  - date: from 2020-09-11 to 2020-09-14
  *  - FEC & LDCH: Keep the max FOV in the horizontal(X direction)
  *  - FEC: 1) Add "correctX" and "correctY" flag:
			   a) correctX = 1, correctY = 1: correct both directions
			   b) correctX = 1, correctY = 0: correct x direction
			   c) correctX = 0, correctY = 1: correct y direction
			   d) correctX = 0, correctY = 0: no correct
			2) Add "saveMesh4bin" flag: whether to save 4 mesh bins(meshxi,meshxf,meshyi,meshyf) or not
			   a) saveMesh4bin = 1: save
			   b) saveMesh4bin = 0: not save
			3) Add "mesh4binPath" : set the path for saving 4 mesh bins, for example, mesh4binPath = "../data_out/"
			   when 4 mesh bins saved, the prefix file names of each group of bins are
			   a) level255_both_correct_: correct both directions
			   b) level255_x_correct_: correct x direction
			   c) level255_y_correct_: correct y direction
			   d) level255_no_correct_: no correct
  *  - LDCH: 1) Add judgment: calculate the max correction capability("maxLevel") of LDCH, and divide it into 255 levels.
                so the value of level can still vary from 0 to 255 when the function is called.
             2) Add "saveMeshX" flag: whether to save mesh bin or not
			   a) saveMeshX = 1: save
			   b) saveMeshX = 0: not save
             3) Add "meshPath" : set the path for saving mesh bin, for example, meshPath = "../data_out/"
  
  * v1.0.1
  *  - date: 2020-09-15
  *  - LDCH: optimize the "calcLdchMaxLevel" function to reduce the cost time,
			 and it takes about 160ms in PC

  * v1.0.2
  *  - date: 2020-09-16
  *  - LDCH: continue to optimize the "calcLdchMaxLevel" function to reduce the cost time,
			 and it takes about 50ms in PC

  * v1.0.3
  *  - date: 2020-09-17
  *  - LDCH: continue to optimize the "calcLdchMaxLevel" function to reduce the cost time,
			 and it takes about 8ms in PC

  * v2.0.0
  *  - date: 2020-09-17
  *  - FEC&LDCH: change to use rho-cotTheta(or rho-tanTheta) in polynomial fitting,
                 and it saves some time in function "genFECMeshNLevel" and "genLDCMeshNLevel" for each level
  *  - LDCH: continue to optimize the "calcLdchMaxLevel" function to reduce the cost time,
			 and it only takes 4ms in PC

  * v2.0.1
  *  - date: 2020-09-18
  *  - FEC&LDCH: due to a bug in polynomial fitting, last version will returns error when the I/O resolutions are different,
                 it has been solved in this version
	 - FEC: the fixed-point number is changed from 7 to 8

  * v2.0.2
  *  - date: 2020-09-23
  *  - FEC&LDCH: adjust some code to facilitate branch prediction logic and reduce unnecessary memory references to improve efficiency

  * v2.0.3
  *  - date: 2020-10-10
  *  - FEC&LDCH: fix some small bugs
  
  * v2.0.4
  *  - date: 2020-10-15
  *  - FEC: change mesh border frome (srcW-3, srcH-3) to (srcW-1, srcH-1)
*/

#define RK_GENMESH_VERSION_REAL_V "v2.0.4"
#define RK_GENMESH_RELEASE_DATE "2020-10-15"

  /******* DO NOT EDIT THE FOLLOWINGS *******/

#define RK_GENMESH_VERSION_HEAD "GENMESH "
#define RK_GENMESH_VERSION \
    RK_GENMESH_VERSION_HEAD\
    RK_GENMESH_VERSION_REAL_V

#endif // !__RKGENMESHVERSION_H__
