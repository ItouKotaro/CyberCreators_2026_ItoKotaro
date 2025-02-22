xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 44;
 -0.49126;-0.91996;3.24759;,
 1.12692;4.60049;-1.22075;,
 0.38876;4.23007;-0.90237;,
 -0.33878;3.29735;-0.90563;,
 -1.30872;0.15657;-0.91364;,
 -1.03946;-0.39468;-0.91416;,
 -0.41293;-0.66858;-0.91342;,
 0.16552;-1.09818;-0.91308;,
 0.64098;-0.98172;-0.91191;,
 1.61091;2.15903;-0.90389;,
 1.59374;3.52655;-1.08191;,
 0.54882;6.21452;-2.65438;,
 -0.04226;4.64662;-2.22284;,
 -0.48766;3.73711;-2.08168;,
 -1.10855;1.72660;-2.08681;,
 -1.52424;0.28443;-2.09043;,
 -0.51779;-0.41275;-2.08974;,
 0.88101;-1.86779;-1.32763;,
 1.48802;0.30965;-1.33332;,
 1.88356;1.88478;-1.56146;,
 1.60629;4.49164;-1.95789;,
 0.54882;6.21452;-2.65438;,
 1.12860;4.32044;-1.73433;,
 0.46231;3.72579;-1.73560;,
 -0.04226;4.64662;-2.22284;,
 -0.22282;3.23342;-1.73915;,
 -0.48766;3.73711;-2.08168;,
 -0.66186;1.81177;-1.74277;,
 -1.10855;1.72660;-2.08681;,
 -0.68834;0.46971;-1.74541;,
 -1.52424;0.28443;-2.09043;,
 -0.68834;0.46971;-1.74541;,
 -0.29215;-0.28012;-1.74606;,
 0.37538;-0.15132;-1.74446;,
 0.88101;-1.86779;-1.32763;,
 0.37538;-0.15132;-1.74446;,
 0.84696;1.09531;-1.74184;,
 1.48802;0.30965;-1.33332;,
 1.32966;2.22752;-1.73794;,
 1.88356;1.88478;-1.56146;,
 1.14551;3.56828;-1.73571;,
 1.60629;4.49164;-1.95789;,
 0.41711;2.01830;-1.18756;,
 -0.29215;-0.28012;-1.74606;;
 
 40;
 3;0,1,2;,
 3;0,2,3;,
 3;0,3,4;,
 3;0,4,5;,
 3;0,5,6;,
 3;0,6,7;,
 3;0,7,8;,
 3;0,8,9;,
 3;0,9,10;,
 3;0,10,1;,
 4;1,11,12,2;,
 4;2,12,13,3;,
 4;3,13,14,4;,
 4;4,14,15,5;,
 4;5,15,16,6;,
 4;6,16,17,7;,
 4;7,17,18,8;,
 4;8,18,19,9;,
 4;9,19,20,10;,
 4;10,20,11,1;,
 4;21,22,23,24;,
 4;24,23,25,26;,
 4;26,25,27,28;,
 4;28,27,29,30;,
 4;15,31,32,16;,
 4;16,32,33,17;,
 4;34,35,36,37;,
 4;37,36,38,39;,
 4;39,38,40,41;,
 4;41,40,22,21;,
 3;22,42,23;,
 3;23,42,25;,
 3;25,42,27;,
 3;27,42,29;,
 3;29,42,43;,
 3;43,42,35;,
 3;35,42,36;,
 3;36,42,38;,
 3;38,42,40;,
 3;40,42,22;;
 
 MeshMaterialList {
  3;
  40;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  1,
  0,
  0,
  1,
  1,
  1,
  1,
  1,
  2,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   8.000000;
   0.100000;0.100000;0.100000;;
   0.070000;0.070000;0.070000;;
   TextureFilename {
    "data/TEXTURE/cat.png";
   }
  }
  Material {
   0.800000;0.778400;0.684000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.756000;0.690400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  46;
  -0.599120;0.545074;0.586473;,
  0.281083;0.637655;0.717209;,
  -0.504212;0.629190;0.591515;,
  -0.843403;0.412615;0.344121;,
  -0.982162;0.011884;0.187661;,
  -0.767968;-0.640440;-0.007874;,
  -0.538645;-0.837329;-0.093501;,
  -0.376092;-0.926514;-0.011233;,
  0.683677;-0.510024;0.521978;,
  0.909464;-0.078141;0.408373;,
  0.835052;0.267928;0.480524;,
  -0.458152;0.685300;0.566093;,
  -0.630352;0.615764;0.472749;,
  -0.863669;0.408242;0.295660;,
  -0.974278;0.101138;0.201378;,
  -0.839163;-0.543637;-0.016287;,
  -0.575044;-0.808487;-0.125190;,
  -0.663299;-0.746697;-0.049776;,
  0.609764;-0.254010;0.750777;,
  0.900615;-0.048158;0.431941;,
  0.885536;0.305917;0.349629;,
  -0.100392;-0.245753;-0.964120;,
  0.272818;-0.325072;-0.905483;,
  0.415791;-0.253714;-0.873354;,
  0.519248;-0.101584;-0.848565;,
  0.447590;0.121047;-0.886008;,
  0.300950;0.225488;-0.926598;,
  0.057943;0.115352;-0.991633;,
  -0.123252;0.046291;-0.991295;,
  -0.180715;-0.014496;-0.983429;,
  -0.346268;-0.102462;-0.932524;,
  -0.061266;0.001015;-0.998121;,
  -0.468245;-0.883582;0.005469;,
  0.860087;0.054508;0.507227;,
  0.387115;-0.303602;0.870614;,
  0.757883;0.500034;0.419021;,
  -0.027705;-0.309320;-0.950554;,
  0.298726;-0.347583;-0.888791;,
  0.476775;-0.279656;-0.833353;,
  0.568666;-0.126793;-0.812738;,
  0.522406;0.171935;-0.835183;,
  0.474778;0.242172;-0.846132;,
  0.412090;-0.024455;-0.910815;,
  0.338201;-0.143768;-0.930027;,
  0.191990;-0.110051;-0.975207;,
  -0.122436;-0.161296;-0.979282;;
  40;
  3;0,1,2;,
  3;0,2,3;,
  3;0,3,4;,
  3;32,4,5;,
  3;32,5,6;,
  3;32,6,7;,
  3;32,7,8;,
  3;33,8,9;,
  3;33,9,10;,
  3;33,10,1;,
  4;1,11,12,2;,
  4;2,12,13,3;,
  4;3,13,14,4;,
  4;4,14,15,5;,
  4;5,15,16,6;,
  4;6,16,17,7;,
  4;34,34,18,8;,
  4;8,18,19,9;,
  4;9,19,20,10;,
  4;10,20,35,1;,
  4;36,21,22,37;,
  4;37,22,23,38;,
  4;38,23,24,39;,
  4;39,24,25,40;,
  4;40,25,26,41;,
  4;41,26,27,42;,
  4;42,27,28,43;,
  4;43,28,29,44;,
  4;44,29,30,45;,
  4;45,30,21,36;,
  3;21,31,22;,
  3;22,31,23;,
  3;23,31,24;,
  3;24,31,25;,
  3;25,31,26;,
  3;26,31,27;,
  3;27,31,28;,
  3;28,31,29;,
  3;29,31,30;,
  3;30,31,21;;
 }
 MeshTextureCoords {
  44;
  0.827137;0.593395;,
  0.378692;0.032955;,
  0.409547;0.070560;,
  0.407935;0.165250;,
  0.405416;0.484105;,
  0.405838;0.540068;,
  0.407018;0.567875;,
  0.408072;0.611488;,
  0.409028;0.599665;,
  0.411548;0.280813;,
  0.393538;0.141982;,
  0.232878;-0.130903;,
  0.275421;0.028271;,
  0.288893;0.120606;,
  0.287280;0.324714;,
  0.286182;0.471124;,
  0.288026;0.541903;,
  0.367464;0.689619;,
  0.367960;0.468564;,
  0.345615;0.308656;,
  0.305087;0.044005;,
  -2.107630;-1.244400;,
  -2.013690;-1.052120;,
  -2.014990;-0.991750;,
  -2.065090;-1.085230;,
  -2.016560;-0.941760;,
  -2.051620;-0.992900;,
  -2.017700;-0.797440;,
  -2.053230;-0.788790;,
  -2.018010;-0.661190;,
  -2.054330;-0.642380;,
  0.322502;0.452315;,
  0.323135;0.528438;,
  0.324473;0.515362;,
  -1.973050;-0.423890;,
  -2.016040;-0.598140;,
  -2.014940;-0.724700;,
  -1.972550;-0.644940;,
  -2.013700;-0.839640;,
  -1.994900;-0.804850;,
  -2.013800;-0.975760;,
  -2.035420;-1.069500;,
  -1.959720;-0.818400;,
  -2.017380;-0.585070;;
 }
}
