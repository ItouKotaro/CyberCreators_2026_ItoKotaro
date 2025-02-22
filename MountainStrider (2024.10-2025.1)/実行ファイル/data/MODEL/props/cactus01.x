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
 49;
 -3.20783;52.30317;-1.18755;,
 -2.15487;49.91578;-4.13840;,
 -4.52261;49.68952;-3.82492;,
 -0.26518;50.15097;-2.67898;,
 0.03948;50.25725;-0.30154;,
 -1.41943;50.17233;1.60113;,
 -3.78729;49.94602;1.91461;,
 -5.67698;49.71090;0.45519;,
 -5.98152;49.60458;-1.92212;,
 -0.78857;43.14746;-6.48874;,
 -5.16367;42.72928;-5.90940;,
 2.70318;43.58204;-3.79203;,
 3.26601;43.77835;0.60084;,
 0.57018;43.62149;4.11666;,
 -3.80493;43.20329;4.69586;,
 -7.29667;42.76874;1.99928;,
 -7.85950;42.57231;-2.39359;,
 0.68295;33.02856;-7.88062;,
 -5.03339;32.48210;-7.12376;,
 5.24517;33.59629;-4.35725;,
 5.98049;33.85283;1.38228;,
 2.45837;33.64796;5.97599;,
 -3.25810;33.10137;6.73285;,
 -7.82019;32.53364;3.20948;,
 -8.55563;32.27699;-2.53005;,
 2.03578;21.09944;-8.10212;,
 -4.15167;20.50801;-7.28302;,
 6.97371;21.71404;-4.28859;,
 7.76976;21.99174;1.92393;,
 3.95735;21.76987;6.89602;,
 -2.23009;21.17844;7.71525;,
 -7.16804;20.56382;3.90158;,
 -7.96407;20.28612;-2.31082;,
 5.28964;9.27730;-6.28349;,
 -0.52133;8.97867;-7.04212;,
 8.69168;9.57212;-1.90284;,
 8.17333;9.74902;3.50041;,
 4.09040;9.62831;7.23961;,
 -1.63766;9.21857;7.18657;,
 -5.73532;8.83504;2.87962;,
 -5.30001;8.76939;-3.22959;,
 5.93349;-0.91194;-3.67082;,
 1.83784;-0.91194;-5.36724;,
 7.63003;-0.91194;0.42495;,
 5.93349;-0.91194;4.52060;,
 1.83784;-0.91194;6.21702;,
 -2.25781;-0.91194;4.52060;,
 -3.95436;-0.91194;0.42495;,
 -2.25781;-0.91194;-3.67082;;
 
 48;
 3;0,1,2;,
 3;0,3,1;,
 3;0,4,3;,
 3;0,5,4;,
 3;0,6,5;,
 3;0,7,6;,
 3;0,8,7;,
 3;0,2,8;,
 4;2,1,9,10;,
 4;1,3,11,9;,
 4;3,4,12,11;,
 4;4,5,13,12;,
 4;5,6,14,13;,
 4;6,7,15,14;,
 4;7,8,16,15;,
 4;8,2,10,16;,
 4;10,9,17,18;,
 4;9,11,19,17;,
 4;11,12,20,19;,
 4;12,13,21,20;,
 4;13,14,22,21;,
 4;14,15,23,22;,
 4;15,16,24,23;,
 4;16,10,18,24;,
 4;18,17,25,26;,
 4;17,19,27,25;,
 4;19,20,28,27;,
 4;20,21,29,28;,
 4;21,22,30,29;,
 4;22,23,31,30;,
 4;23,24,32,31;,
 4;24,18,26,32;,
 4;26,25,33,34;,
 4;25,27,35,33;,
 4;27,28,36,35;,
 4;28,29,37,36;,
 4;29,30,38,37;,
 4;30,31,39,38;,
 4;31,32,40,39;,
 4;32,26,34,40;,
 4;34,33,41,42;,
 4;33,35,43,41;,
 4;35,36,44,43;,
 4;36,37,45,44;,
 4;37,38,46,45;,
 4;38,39,47,46;,
 4;39,40,48,47;,
 4;40,34,42,48;;
 
 MeshMaterialList {
  4;
  48;
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
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\PROP\\cactus001.jpg";
   }
  }
  Material {
   0.800000;0.000000;0.401600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.793600;0.800000;0.715200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  49;
  -0.099265;0.994555;-0.031736;,
  -0.453659;0.542332;-0.707156;,
  0.146722;0.599720;-0.786644;,
  0.625856;0.659346;-0.416614;,
  0.703091;0.686291;0.186194;,
  0.333171;0.664754;0.668655;,
  -0.267194;0.607367;0.748139;,
  -0.746342;0.547744;0.378087;,
  -0.823574;0.520795;-0.224716;,
  -0.503402;0.194295;-0.841924;,
  0.223920;0.263825;-0.938220;,
  0.804379;0.336063;-0.489935;,
  0.897945;0.368695;0.240332;,
  0.449805;0.342608;0.824800;,
  -0.277515;0.273082;0.921093;,
  -0.857972;0.200843;0.472807;,
  -0.951535;0.168208;-0.257465;,
  -0.502992;0.035123;-0.863577;,
  0.247592;0.106873;-0.962952;,
  0.846615;0.181421;-0.500329;,
  0.943173;0.215099;0.253293;,
  0.480704;0.188179;0.856453;,
  -0.269886;0.116427;0.955828;,
  -0.868903;0.041877;0.493207;,
  -0.965462;0.008201;-0.260416;,
  -0.439838;-0.083116;-0.894223;,
  0.320328;0.004573;-0.947296;,
  0.889761;0.087531;-0.447954;,
  0.944813;0.117343;0.305874;,
  0.459858;0.080260;0.884358;,
  -0.295391;-0.002574;0.955373;,
  -0.883018;-0.086259;0.461344;,
  -0.942866;-0.120914;-0.310458;,
  -0.255199;-0.172613;-0.951356;,
  0.497908;-0.104912;-0.860860;,
  0.962229;-0.044333;-0.268607;,
  0.880716;-0.025749;0.472945;,
  0.310400;-0.060468;0.948681;,
  -0.439724;-0.129395;0.888763;,
  -0.934578;-0.191657;0.299720;,
  -0.856245;-0.209026;-0.472390;,
  -0.129582;-0.201805;-0.970816;,
  0.601236;-0.169716;-0.780840;,
  0.980543;-0.140261;-0.137339;,
  0.805166;-0.129382;0.578764;,
  0.184440;-0.151081;0.971162;,
  -0.545658;-0.194250;0.815184;,
  -0.958781;-0.225942;0.172307;,
  -0.784323;-0.225738;-0.577823;;
  48;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,5,4;,
  3;0,6,5;,
  3;0,7,6;,
  3;0,8,7;,
  3;0,1,8;,
  4;1,2,10,9;,
  4;2,3,11,10;,
  4;3,4,12,11;,
  4;4,5,13,12;,
  4;5,6,14,13;,
  4;6,7,15,14;,
  4;7,8,16,15;,
  4;8,1,9,16;,
  4;9,10,18,17;,
  4;10,11,19,18;,
  4;11,12,20,19;,
  4;12,13,21,20;,
  4;13,14,22,21;,
  4;14,15,23,22;,
  4;15,16,24,23;,
  4;16,9,17,24;,
  4;17,18,26,25;,
  4;18,19,27,26;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,24,32,31;,
  4;24,17,25,32;,
  4;25,26,34,33;,
  4;26,27,35,34;,
  4;27,28,36,35;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,31,39,38;,
  4;31,32,40,39;,
  4;32,25,33,40;,
  4;33,34,42,41;,
  4;34,35,43,42;,
  4;35,36,44,43;,
  4;36,37,45,44;,
  4;37,38,46,45;,
  4;38,39,47,46;,
  4;39,40,48,47;,
  4;40,33,41,48;;
 }
 MeshTextureCoords {
  49;
  1.819426;-11.237192;,
  0.564351;-10.724967;,
  1.489752;-10.671763;,
  0.323909;-10.766296;,
  0.909291;-10.771526;,
  1.977578;-10.737585;,
  2.903020;-10.684371;,
  3.143463;-10.643057;,
  2.558076;-10.637817;,
  -0.671191;-9.225711;,
  1.038787;-9.127378;,
  -1.115474;-9.302078;,
  -0.033812;-9.311726;,
  1.940209;-9.249022;,
  3.650149;-9.150686;,
  4.094467;-9.074325;,
  3.012804;-9.064651;,
  -1.699022;-6.967655;,
  0.535145;-6.839160;,
  -2.279531;-7.067417;,
  -0.866262;-7.080033;,
  1.712888;-6.998125;,
  3.947100;-6.869600;,
  4.527563;-6.769838;,
  3.114335;-6.757197;,
  -2.362700;-4.294535;,
  0.055555;-4.155466;,
  -2.991015;-4.402539;,
  -1.461326;-4.416199;,
  1.330328;-4.327515;,
  3.748617;-4.188446;,
  4.376898;-4.080438;,
  2.847237;-4.066777;,
  -3.103121;-1.629452;,
  -1.274072;-1.568386;,
  -3.027039;-1.661304;,
  -1.269559;-1.658684;,
  1.259872;-1.602329;,
  3.263856;-1.510829;,
  3.454067;-1.458505;,
  1.521592;-1.491593;,
  -2.673787;0.675718;,
  -1.720618;0.662472;,
  -2.081102;0.707756;,
  -0.289758;0.739818;,
  1.650863;0.753121;,
  2.604033;0.739874;,
  2.011386;0.707837;,
  0.220003;0.675775;;
 }
}
