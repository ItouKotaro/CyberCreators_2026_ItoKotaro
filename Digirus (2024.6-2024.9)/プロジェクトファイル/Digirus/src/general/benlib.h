//==================================================================
//
// 便利関数ライブラリ [benlib.h]
// by Kotaro Ito
//
//==================================================================
#ifndef _BENLIB_H_
#define _BENLIB_H_

#include <stdio.h>
#include "d3dx9.h"	

/*
@brief 座標1と座標2の距離を計算します
@param[in] pos1 : 座標1
@param[in] pos2 : 座標2
@return 距離 (float)
*/
float PosDistance(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2);

/*
@brief 座標1と座標2の平面距離を計算します
@param[in] pos1 : 座標1
@param[in] pos2 : 座標2
@return 平面距離 (float)
*/
float PosPlaneDistance(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2);

/*
@brief 方向ベクトルと長さから座標を計算します
@param[in] start : スタート位置
@param[in] vec : 方向ベクトル
@param[in] fLength : 距離
@return 座標 (D3DXVECTOR3)
*/
D3DXVECTOR3 VectorDistance(const D3DXVECTOR3& start, D3DXVECTOR3 vec, const float& fLength);

/*
@brief 座標1から座標2の角度を計算します
@param[in] pos1 : 座標1
@param[in] pos2 : 座標2
@return 角度 (float)
*/
float PosAngle(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2);

/*
@brief 座標1から座標2の回転を計算します
@param[in] pos1 : 座標1
@param[in] pos2 : 座標2
@return 回転 (D3DXVECTOR3)
*/
D3DXVECTOR3 PosRotation(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2);

/*
@brief 角度からラジアンに変換します
@param[in] nDegree : 角度
@return ラジアン (float)
*/
float DegreeToRadian(const int& nDegree);

/*
@brief ラジアンから角度に変換します
@param[in] fRadian : ラジアン
@return 角度 (int)
*/
int RadianToDegree(const float& fRadian);

/*
@brief クォータニオンからYXZオイラー角に変換します
@param[in] q : クォータニオン
@return オイラー角（YXZ）
*/
D3DXVECTOR3 QuaternionToEuler(D3DXQUATERNION q);

/*
@brief ファイルポインタを現在位置から指定数分戻します
@param[in] pFile : ファイルポインタ
@param[in] nBackCount : 戻る文字数
*/
void BackFileChar(FILE* pFile, const int& nBackCount);

/*
@brief パスを指定数分削除します
@param[out] FilePath : パス (文字列)
@param[in] nSplit : パス数
*/
void RemovePathSplit(char* FilePath, const int& nSplit);
void RemovePathSplit(std::string& FilePath, const int& nSplit);

/*
@brief パスの区切りを指定文字に置き換える
@param[out] FilePath : パス (文字列)
@param[in] cSplit : 新しい区切り文字
*/
void ReplacePathSplit(char* FilePath, const char& cSplit);

#endif