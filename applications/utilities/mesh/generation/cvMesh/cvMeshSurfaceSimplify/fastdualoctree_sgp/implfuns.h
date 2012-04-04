/**
 * @file    implfuns.h
 * @author  Thomas Lewiner 
 * @version 
 * @date    
 *
 * @brief
 *
 */
//________________________________________________


#pragma once

#define NFUNS 38

// implicit functions
const char *fun_list[NFUNS] =
{
  "Type Formula"    ,
  "Sphere"          ,
  "Ellipsoid"       ,
  "Hyperboloid"     ,
  "Plane"           ,
  "Cubic"           ,
  "Cushin"          ,
  "Cassini"         ,
  "Tangle cube"     ,
  "Chair"           ,
  "Cyclide"         ,
  "2 Spheres"       ,
  "2 Torii"         ,
  "Heart"           ,
  "Smile"           ,
  "Spheres in"      ,
  "Spheres dif"     ,
	"Cross cap"       ,
  "Weird cube"      ,
  "Klein bottle"    ,
  "Steiner's roman" ,
	"Hunt's surface"  ,
  "Cayley cubic"    ,
  "Clebsch cubic"   ,
  "Barth sextic"    ,
	"Barth decic"     ,
  "Steiner relative",
  "Mitre"           ,
  "Glob tear"       ,
	"Piri tear"       ,
  "Gumdrop torus"   ,
  "Bretzel"         ,
  "Blob"            ,
  "Bifolia"         ,
  "Lemniscate"      ,
	"Planes"          ,
  "Cylinders"       ,
  "Torus"           ,
} ;

// implicit functions
const char *fun_def [NFUNS] =
{
  /* Type Formula     */  "f(x,y,z)",
  /* Sphere           */  "4*x^2-4*x+2.51+4*y^2-4*y+4*z^2-4*z",
  /* Ellipsoid        */  "8*x^2-8*x+3.51+4*y^2-4*y+4*z^2-4*z",
  /* Hyperboloid      */  "8*x^2-8*x-.1-4*y^2+4*y-4*z^2+4*z",
  /* Plane            */  "x+y+z",
  /* Cubic            */  "16*y^2-16*y+10-64*x^3+96*x^2-44*x",
  /* Cushin           */  "112.5*z+81*y*z^2+51.75*x+119.25*y-135*y*z-38.8125+162*x^3-81*x^4+162*y^3+216*z^3-81*y^4-81*z^4-162*x*y-132.75*x^2-200.25*y^2-220.5*z^2+162*x*y^2+81*x*z-81*x*z^2-81*x^2*z-162*x^2*y^2-81*y^2*z^2+162*x^2*y+135*y^2*z+81*z^2*x^2",
  /* Cassini          */  "-167.6778*z-267.2672*y*z^2-167.6778*x-205.1322*y+267.2672*y*z+59.74405625-267.2672*x^3+133.6336*x^4-267.2672*y^3-267.2672*z^3+133.6336*y^4+133.6336*z^4+267.2672*x*y+301.3114*x^2+338.7658*y^2+301.3114*z^2-267.2672*x*y^2+267.2672*x*z-267.2672*x*z^2-267.2672*x^2*z+267.2672*x^2*y^2+267.2672*y^2*z^2-267.2672*x^2*y-267.2672*y^2*z+267.2672*z^2*x^2",
  /* Tangle cube      */  "-468*z-468*x-468*y+1296*y^4+1764*x^2+1764*y^2+1764*z^2+1296*z^4-2592*x^3+119.8-2592*y^3+1296*x^4-2592*z^3",
  /* Chair            */  "-18250*z-36000*y*z^2-2250*x-18250*y+52000*y*z+4626.5625-20000*x^3+10000*x^4-20000*y^3-4000*z^3+10000*y^4+2000*z^4-12000*x*y+12250*x^2+28250*y^2+20250*z^2+12000*x*y^2+20000*x*z-36000*x*z^2-20000*x^2*z-12000*x^2*y^2+36000*y^2*z^2+12000*x^2*y-52000*y^2*z+36000*z^2*x^2",
  /* Cyclide          */  "-11.23227514*x^2-6.0015984*x^4-6.0015984*y^4-6.0015984*z^4-13.56959251*z^2+12.0031968*y*z^2+12.0031968*z^3+6.027648307*x+7.256824512*y-13.25842291*y^2+12.0031968*y^3+12.0031968*x^2*z-9.79449408*x*y+9.79449408*x*y^2-9.79449408*x*z+9.79449408*x*z^2-12.0031968*x^2*y^2-12.0031968*y^2*z^2-12.0031968*z^2*x^2+7.567994112*z+12.0031968*y^2*z+12.0031968*x^2*y-12.0031968*y*z-2.289346823+9.79449408*x^3",
  /* 2 Spheres        */  "(40000*x^2-52400*x+48853+40000*y^2-52400*y+40000*z^2-52400*z)*(4000*x^2-2800*x+1207+4000*y^2-2800*y+4000*z^2-2800*z)",
  /* 2 Torii          */  "-6.929911075*x -6.559601218*z -40.61779736*z^3 +56.91300912*z^4 -17.17986918*y^7 +4.294967296*y^8 -55.95084095*z^5 +38.69346103*z^6 +38.69346103*x^6 +4.294967296*z^8 -17.17986918*x^7 +37.0828483*y^6 +4.294967296*x^8 -17.17986918*z^7 +20.40667127*z^2 -51.53960755*y^3*z^4 -89.2368375*x^4*y -41.69153918*x^3 +51.53960755*x*z^5 -51.11900275*y^5 -51.53960755*x^2*z^5 -51.53960755*y^5*z^2 +92.57432908*x^3*y +87.74249087*y^3*z -5.938337301*y +17.84876088*y^2 -35.13389168*y^3 +1.174097113 -51.53960755*y^2*z^5 +51.53960755*x^5*y -62.4313134*x*y^2 +50.14452444*y^4 +51.53960755*y^5*z -67.27041824*x^2*z -62.06100354*y*z^2 +114.4697704*y^2*z^4 +17.17986918*y^2*z^6 +88.81623269*y^3*x +112.8591576*y^4*z^2 +114.4697704*x^4*y^2 -87.08935385*y^4*z +25.76980378*y^4*z^4 +103.0792151*x^3*z^3 +103.0792151*y^3*z^3 -87.08935385*y^4*x +112.8591576*y^4*x^2 +90.42684543*y*z^3 +51.53960755*y*z^5 +116.0803831*x^2*z^4 +17.17986918*x^2*z^6 -146.2614203*x^3*z^2 +94.72181273*x^3*z +25.76980378*x^4*z^4 -51.53960755*x^4*z^3 -88.16309567*y*z^4 +25.43751542*y*z -90.31057932*x*z^4 -90.31057932*x^4*z +116.0803831*x^4*z^2 -139.2820984*y^3*z^2 -17.17986918*y^6*z -140.3558402*y^3*x^2 +17.17986918*x^6*z^2 +94.72181273*x*z^3 +17.17986918*y^6*z^2 -143.0401948*x^3*y^2 -17.17986918*x^6*z -17.17986918*x*z^6 -51.53960755*x^3*z^4 +51.53960755*x^5*z -51.53960755*x^5*z^2 +103.0792151*x^3*y^3 +17.17986918*x^6*y^2 -17.17986918*x^6*y +51.53960755*y^5*x -17.17986918*y*z^6 -51.53960755*y^5*x^2 -17.17986918*y^6*x +17.17986918*y^6*x^2 +25.76980378*x^4*y^4 -143.0401948*y^2*z^3 -146.2614203*x^2*z^3 +26.71500605*x*y +28.49944647*x*z -67.27041824*x*z^2 +125.3614762*x^2*y^2 +123.7508635*y^2*z^2 -60.82070066*y^2*z +131.8111938*z^2*x^2 +21.31385204*x^2 +51.53960755*y^4*z*x +103.0792151*y^2*z^3*x +51.53960755*y^2*z^2*x^4 -51.53960755*y^4*z^2*x +103.0792151*y^3*z^2*x -103.0792151*y^2*z^2*x^3 -51.53960755*x^4*y^2*z +103.0792151*x^2*z^3*y -51.53960755*x^2*z^4*y +103.0792151*x^3*y^2*z -51.53960755*y^2*z^4*x +51.53960755*y^4*z^2*x^2 -103.0792151*y^3*z^2*x^2 +51.53960755*y^2*z^4*x^2 -103.0792151*y^2*z^3*x^2 +51.53960755*x*z^4*y +51.53960755*x^4*y*z -51.53960755*y^4*z*x^2 -103.0792151*x^3*y*z -51.53960755*x^4*y*z^2 +103.0792151*x^3*y*z^2 -103.0792151*x*z^3*y -103.0792151*y^3*z*x +103.0792151*y^3*z*x^2 +125.8603256*y*z^2*x -177.3999332*x^2*y*z^2 -177.3999332*x^2*z*y^2 +125.8603256*y^2*z*x -177.3999332*x*y^2*z^2 +228.9395407*y^2*z^2*x^2 -74.32071807*y*z*x +125.8603256*y*z*x^2 -64.41223599*x^2*y -51.53960755*x^5*y^2 +57.44988003*x^4 -55.95084095*x^5 -51.53960755*x^4*y^3 -51.53960755*x^3*y^4 -51.53960755*y^4*z^3",
  /* Heart            */  "-697.347066*z-2684.277024*y*z^2-1347.168628*x-687.693448*y+926.747328*y^2*z^4+3706.989312*y^3*x+926.747328*y^4*z^2+3706.989312*x^4*y^2-926.747328*y^4*z-1853.494656*y^4*x+1853.494656*y^4*x^2+1972.308416*y*z^3+1853.494656*x^2*z^4-7413.978624*x^3*z^2+7413.978624*x^3*z-926.747328*y*z^4+1668.419376*y*z-1853.494656*x*z^4+7413.978624*x^3*y-3706.989312*x^4*z+3706.989312*x^4*z^2+1853.494656*y^3*z-1853.494656*y^3*z^2-3706.989312*y^3*x^2+3718.870688*x*z^3-7413.978624*x^3*y^2-3706.989312*x^4*y-1972.308416*y^2*z^3-3718.870688*x^2*z^3+195.187199+3706.989312*y*z^2*x-3706.989312*x^2*y*z^2-3706.989312*x^2*z*y^2+3706.989312*y^2*z*x-3706.989312*x*y^2*z^2+3706.989312*y^2*z^2*x^2-8788.562432*x^3+10572.59674*x^4-1888.224832*y^3-1920.898616*z^3+1716.401856*y^4+1716.401856*z^4-926.747328*z^5+308.915776*z^6-7413.978624*x^5+2471.326208*x^6-926.747328*y^5+308.915776*y^6+3158.618112*x*y+4505.78674*x^2+1477.347976*y^2+1511.506932*z^2-3706.989312*y*z*x-5012.112768*x*y^2+3706.989312*y*z*x^2+3167.529144*x*z-5029.934832*x*z^2-6874.518456*x^2*z+8719.10208*x^2*y^2+3611.024352*y^2*z^2-6865.607424*x^2*y-2595.166704*y^2*z+8736.924144*z^2*x^2",
  /* Smile            */  "-12348*z-435632*y^3-27216*y*z^2-17834.8125*x-35974.25*y-327680*y^7+733184*y^6-891648*y^4*x+11664*y*z^3+972*x^3*z+21168*y*z+2304*y^3*z+8748*x*z^3+46656*x^6*y^2-58320*x^6*y+5162.816406-1100160*y^3*x^2+11664*y*z^2*x+5184*y^2*z*x-72987.75*x^3+95610.375*x^4-20412*z^3+6561*z^4+65536*y^8-89667*x^5+60507*x^6-26244*x^7+6561*x^8+41910.1875*x^2+23814*z^2+712044*x^2*y^2-257499*x^2*y-18144*y*z*x+3888*y*z*x^2-311040*x^4*y^3+174960*x^5*y-139968*x^5*y^2+622080*x^3*y^3+552960*y^5*x-552960*y^5*x^2-147456*y^6*x+1016064*y^4*x^2+789888*y^3*x+340632*x^3*y-957440*y^5-315900*x^4*y-408060*x*y^2-248832*x^3*y^4+147456*y^6*x^2+124416*x^4*y^4-660960*x^3*y^2+155243*y^2+797792*y^4+447120*x^4*y^2+120591*x*y+15876*x*z-20412*x*z^2-6804*x^2*z+7776*y^2*z^2-12096*y^2*z+4374*z^2*x^2",
  /* Spheres in       */  "-(8*(4*x^2-4*x+3+4*y^2-4*y+4*z^2-4*z))*(-1+sqrt(4*x^2-4*x+3+4*y^2-4*y+4*z^2-4*z))",
  /* Spheres dif      */  "-496*z-4416*y*z^2-560*x-560*y+6912*y^2*z^4+1024*y^2*z^6+9728*y^3*x+7296*y^4*z^2+13056*x^4*y^2-7680*y^4*z-6144*x^4*y^3+1536*y^4*z^4+6144*x^3*z^3+6144*y^3*z^3-9984*y^4*x+13056*y^4*x^2+5760*y*z^3+3072*y*z^5-3072*x^2*z^5+6912*x^2*z^4+1024*x^2*z^6-9472*x^3*z^2+6144*x^5*y-6144*x^5*y^2+7680*x^3*z+1536*x^4*z^4-3072*x^4*z^3-5376*y*z^4-3072*y^3*z^4+4608*y^4*z*x+6144*y^2*z^3*x+3072*y^2*z^2*x^4-3072*y^4*z^2*x+6144*y^3*z^2*x-6144*y^2*z^2*x^3-4608*x^4*y^2*z+6144*x^2*z^3*y-3072*x^2*z^4*y+9216*x^3*y^2*z-3072*y^2*z^4*x+3072*y^4*z^2*x^2-6144*y^3*z^2*x^2+3072*y^2*z^4*x^2-6144*y^2*z^3*x^2+3072*x*z^4*y+4608*x^4*y*z-4608*y^4*z*x^2-9216*x^3*y*z-3072*x^4*y*z^2+6144*x^3*y*z^2-6144*x*z^3*y-9216*y^3*z*x+9216*y^3*z*x^2+2112*y*z-5376*x*z^4+9728*x^3*y-7680*x^4*z+7296*x^4*z^2+7680*y^3*z-9472*y^3*z^2-1536*y^6*z-15872*y^3*x^2+1024*x^6*z^2+5760*x*z^3+1024*y^6*z^2-3072*y^4*z^3-15872*x^3*y^2-1536*x^6*z+3072*x*z^5-1024*x*z^6-3072*x^3*z^4+4608*x^5*z-3072*x^5*z^2+12288*x^3*y^3+2048*x^6*y^2-2048*x^6*y+6144*y^5*x-1024*y*z^6-6144*y^5*x^2+4608*y^5*z-3072*y^5*z^2-9984*x^4*y-2048*y^6*x-6144*x^3*y^4+2048*y^6*x^2+3072*x^4*y^4-8832*y^2*z^3-8832*x^2*z^3-3072*y^2*z^5+96+8448*y*z^2*x-11520*x^2*y*z^2-15360*x^2*z*y^2+10752*y^2*z*x-11520*x*y^2*z^2+14592*y^2*z^2*x^2-3776*x^3+5600*x^4-3776*y^3-2560*z^3+5600*y^4+3440*z^4-2048*y^7+512*y^8-3328*z^5+2304*z^6-5888*x^5+4352*x^6-5888*y^5+256*z^8-2048*x^7+4352*y^6+512*x^8-1024*z^7+2496*x*y+1808*x^2+1808*y^2+1392*z^2-6144*y*z*x-6336*x*y^2+10752*y*z*x^2+2112*x*z-4416*x*z^2-5184*x^2*z+13248*x^2*y^2+8640*y^2*z^2-6336*x^2*y-5184*y^2*z+8640*z^2*x^2",
  /* Cross cap        */  ".4*sqrt(100*x-10)*x^2-0.08*sqrt(100*x-10)*x+0.008*sqrt(100*x-10)+.4*sqrt(100*x-10)*y^2-0.08*sqrt(100*x-10)*y+1.6*sqrt(100*x-10)*z^2-.8*sqrt(100*x-10)*z+.1*sqrt(100*y-10)*y^2-0.02*sqrt(100*y-10)*y+0.001*sqrt(100*y-10)+.4*sqrt(100*y-10)*z^2-.4*sqrt(100*y-10)*z",
  /* Weird cube       */  "59136*z-10240*y^3-10240*x^3+59136*x+59136*y+204800*z^2*x^2+204800*y^2*z^2-23232+204800*x^2*y^2-102400*x^2*y-102400*x^2*z-297216*x*z+128000*z^2-512000*y*z*x^2+128000*x^2+1566720*y*z*x-102400*x*z^2-512000*y*z^2*x-512000*y^2*z*x-102400*z^3-297216*y*z+102400*y^3*x-102400*y*z^2+102400*x^3*z+102400*x^3*y-102400*x*y^2+128000*y^2+102400*y*z^3-297216*x*y+102400*y^3*z-102400*y^2*z+102400*x*z^3",
  /* Klein bottle     */  "-244615.68*y^3-304335.36*x^3-96138.60864*y-786432*z^5-453427.2*y^3*z^2+1139015.68*z^4-139968*y^4*x-255052.8*y^3*x^2+248832*x^4*z^2-248832*x^4*z+33252.70426+46656*x^6+46656*y^6+1051729.92*y^2*z^2+493102.08*x^2*y^2+442368*x^2*z^4-497664*x^3*z^2-403046.4*y*z^4-442368*x*z^4-344176.128*x^2*y-705392.64*x^2*z+497664*y^2*z^2*x^2+480030.72*x*z-484761.6*y*z*x+555332.4032*z^2+453427.2*y*z*x^2+243162.3168*x^2-931184.64*x*z^2+453427.2*y*z^2*x+525312*y^2*z*x-991887.36*z^3+392220.672*y*z+255052.8*y^3*x-779599.872*y*z^2+525312*x^3*z+255052.8*x^3*y-127526.4*y^5-127526.4*x^4*y-366958.08*x*y^2-497664*x*y^2*z^2+262144*z^6-497664*x^2*z*y^2-453427.2*x^2*y*z^2+1106288.64*z^2*x^2-139968*x^5-279936*x^3*y^2-884736*x^2*z^3-120681.6768*x+196337.664*y^2-191443.7632*z+231206.4*y^4+261895.68*x^4-884736*y^2*z^3+806092.8*y*z^3+442368*y^2*z^4+248832*y^4*z^2+139968*x^4*y^2-248832*y^4*z+139968*y^4*x^2+232316.928*x*y+453427.2*y^3*z-623185.92*y^2*z+933888*x*z^3",
  /* Steiner's roman  */  "0.03125*y^2+.125*y*z*x-0.00390625-0.0625*y^2*z-0.0625*y*z^2+0.03125*z^2-0.0625*x^2*z+0.03125*x^2+0.0625*x^2*y^2+0.0625*z^2*x^2+0.0625*y^2*z^2-0.0625*x^2*y-0.0625*x*y^2-0.0625*x*z^2",
  /* Hunt's surface   */  "-321408*y-559872*z^5+1446336*z^4+933120*z^2*x^2-1990656*y^2*z^3+559872*x^4*z^2+1990656*y*z^2*x-559872*x^4*z-1990656*x^3*y^2-3538944*y^3*x^2+1119744*x*z^3-3538944*y^3*z^2-1959552*z^3-1469664*x^3+37719-1119744*x^3*z^2+1119744*x^3*z-1658880*y*z^2-1769472*y^4*z+1990656*y*z^3+1990656*x^3*y+3538944*y^3*z+3428352*y^2*z^2+1534464*x*y+186624*x^6+559872*x^2*z^4-995328*y*z^4-559872*x*z^4-373248*x^2*z-186624*x*z-3334144*y^3+4288512*y^4+1201392*x^4+1990656*y^2*z*x-1990656*x*y^2*z^2+3538944*y^3*x+1769472*y^4*z^2+1769472*y^4*x^2-1769472*y^4*x+1990656*y*z*x^2+802872*x^2-1990656*y*z*x-3303936*x*y^2-2433024*y^2*z-373248*x*z^2+663552*y*z-995328*x^4*y+4299264*x^2*y^2+1990656*y^2*z^2*x^2+186624*z^6-559872*x^5-3145728*y^5+1048576*y^6-2529792*x^2*y-161352*x-229392*z-1990656*x^2*y*z^2-1990656*x^2*z*y^2+1115856*z^2+995328*y^2*z^4+995328*x^4*y^2-1119744*x^2*z^3+1464192*y^2",
  /* Cayley cubic     */  "264-512*z-512*x-512*y+320*y^2-320*x^2*y-320*x^2*z-320*x*y^2+672*x*y-320*x*z^2+672*x*z-320*y^2*z-320*y*z^2+672*y*z+320*z^2+320*x^2",
  /* Clebsch cubic    */  "8154-15300*z-15300*x-15300*y+4176*y^2+4176*z^2+24480*x*y+24480*y*z+5184*x^3-12096*x*y^2-12096*y^2*z-12096*x*z^2-12096*y*z^2+24480*x*z-12096*x^2*y-12096*x^2*z+4176*x^2+3456*y*z*x+5184*z^3+5184*y^3",
  /* Barth sextic     */  "-2846.637681*z-75426.87093*y*z^2-2846.637681*x-2846.637681*y+224595.2132*y^3*z^2+42893.86887*y^2*z^4+85787.73774*y^3*x-42893.86887*y^4*x+42893.86887*y^4*x^2+85787.73774*y*z^3-85787.73774*x^3*z^2+85787.73774*x^3*z-42893.86887*y*z^4+32533.00206*y*z-42893.86887*x^4*z+42893.86887*x^4*z^2-85787.73774*y^3*x^2-85787.73774*y^2*z^3+571.8691769+36870.73567*x^3-18435.36784*x^4+36870.73567*y^3+36870.73567*z^3-18435.36784*y^4-18435.36784*z^4+32533.00206*x*y-15588.73016*x^2-15588.73016*y^2-15588.73016*z^2-112297.6066*x^2*z^4+112297.6066*x*z^4+277614.9509*y^2*z^2*x^2-75426.87093*x*y^2-277614.9509*y*z*x+277614.9509*y*z*x^2+277614.9509*y*z^2*x+277614.9509*y^2*z*x-224595.2132*x^3*y+112297.6066*x^4*y-277614.9509*x*y^2*z^2-277614.9509*x^2*z*y^2-277614.9509*x^2*y*z^2+224595.2132*x^3*y^2+224595.2132*x^2*z^3-112297.6066*y^4*z^2-112297.6066*x^4*y^2+112297.6066*y^4*z+32533.00206*x*z+79764.60454*x*z^2-75426.87093*x^2*z-36870.73567*x^2*y^2-36870.73567*y^2*z^2-224595.2132*y^3*z-224595.2132*x*z^3+79764.60454*x^2*y+79764.60454*y^2*z-36870.73567*z^2*x^2",
  /* Barth decic      */  "25.33143961*y^2*z^2*x^4 +25.15700632*y^4*z^2*x -34.82908387*y^2*z^4 +8.917960033*y^4*z^2 +0.04360832264*y^4*z^4 +16.49570037*y^4*z +8.917960033*x^4*y^2 -42.37063678*x^4*y^3 -34.82908387*y^4*x^2 +2.299854983*y^7*z^2 +25.37005208*x^4*y^5 +33.65896395*x^6*y^2*z -100.9768918*x^3*y^2*z^4 +1.198101193*x^3 +246.5161156*y^2*z^2*x^2 +25.36672152*x^3*y^4 +6.515326172*y^6*x^2 +44.38789864*y^2*z*x +6.273208191*y^5*z^2 +0.05111641659 +201.9537837*x^2*z^3*y^3 -8.109285345*x^3*z^2 +117.9808071*y^2*z^3*x +0.8487421525*x^8 -8.109285345*y^3*x^2 +6.515326172*x^6*z^2 +33.83339724*x^3*y^3 +50.48844592*x^4*y^2*z^4 -33.65896395*y^6*z*x +100.9768918*x^5*y*z +6.273208191*x^2*z^5 +100.9768918*x^4*y^3*z -201.9537837*x^3*y*z^3 -4.774233544*x^2*z^6 -21.54557109*x^5*y -3.716625642*x^3*z +6.273208191*x^5*y^2 +0.04360832264*x^4*z^4 -100.9768918*x^2*z^3*y^4 +33.65896395*y^2*z^6*x +16.49570037*x*z^4 +16.49570037*x^4*y -120.2077842*x^2*y*z^2 +9.865021177*x*z^6 -42.37063678*x^3*z^4 -3.39496861*x^7 -15.2723629*x^4*z^5 -2.299854983*x*z^7 +25.15700632*x^2*z^4*y +117.9808071*x^3*y^2*z +25.15700632*y^2*z^4*x +50.48844592*x^2*z^4*y^4 -25.60810291*x^3*y^2 -14.9720102*x^6*z -100.9768918*x^5*y^2*z -100.9768918*x^2*z^4*y^3 +6.515326172*y^2*z^6 -3.716625642*y^3*x -5.046246117*y*z^2 -100.9768918*x^4*y^3*z^2 -50.48844592*y^4*z^4*x +100.9768918*x^3*y^4*z +2.299854983*x^7*y^2 -3.940860141*y^8*x +5.090787632*y^6*z^4 -15.2723629*x^5*y^4 +0.8487421525*y^8 +2.299854983*x^2*z^7 -0.5749637458*x^2*z^8 -75.64545224*y^4*z*x -100.9768918*y^5*z*x^2 -50.48844592*x^4*y*z^4 +25.33143961*y^4*z^2*x^2 +1.198101193*z^3 +100.9768918*y^5*z*x +0.9611245006*y^5 -120.2077842*x^2*z*y^2 +3.940860141*y^2*z^8 -10.18157526*x^6*y^3 -8.456684027*x^4*y^6 +201.9537837*x^3*y^2*z^3 -100.9768918*x^2*z^5*y +19.60032929*y*z^4 -15.76344056*x^7*z^2 -3.940860141*x^8*z +0.2347202588*x^3*y +19.60032929*x^4*z +9.865021177*y^6*z -19.05645904*y*z*x -10.25601137*x^5*z +9.865021177*x^6*y +0.4659238756*x^2 -0.3389589536*x -0.3389589536*y +1.198101193*y^3 -3.380386036*y^4 -3.380386036*x^4 -8.109285345*y^2*z^3 -25.60810291*x^2*z^3 -4.774233544*x^6*y^2 +117.9808071*y^3*z^2*x -218.9576989*y^2*z^2*x^3 +25.15700632*x^4*y^2*z +117.9808071*x^2*z^3*y +100.9768918*x^2*z^5*y^2 +33.65896395*y^6*z*x^2 -33.65896395*x^6*y*z +100.9768918*y^4*z^3*x -50.48844592*x^4*y^4*z -50.74010416*x^3*y^5 -3.39496861*y^7 -5.892963096*x*z^2 -5.046246117*x^2*z -100.9768918*y^5*z^2*x -100.9768918*y^2*z^5*x -21.54557109*y^5*z +0.9611245006*z^5 +18.5125231*z^2*x^2 +25.15700632*x^4*y*z^2 -17.00391526*x^3*y*z -33.65896395*x*z^6*y +100.9768918*y^3*z^4*x +5.090787632*x^4*z^6 -34.82908387*x^4*z^2 +2.567983615*x*z +100.9768918*x^3*y*z^4 +3.940860141*x^8*z^2 -8.456684027*x^6*z^4 +16.91336805*y^3*z^6 +30.54472579*y^5*z^3 -15.76344056*y^7*x^2 +15.76344056*y^7*x +30.54472579*x^3*z^5 +25.37005208*x^5*z^4 +25.37005208*y^4*z^5 -10.18157526*y^6*z^3 +33.65896395*x^2*z^6*y -100.9768918*x^4*y^2*z^3 -15.2723629*y^5*z^4 -50.74010416*y^3*z^5 -100.9768918*x^3*y^4*z^2 +50.48844592*x^4*y^4*z^2 -201.9537837*x^3*y^3*z +0.5749637458*y^8*z -0.5749637458*y^8*z^2 +35.62606345*y^5*x^2 -100.9768918*x^5*y*z^2 +3.640421878*z^6 +0.9611245006*x^5 -21.54557109*x*z^5 +0.4659238756*y^2 -33.65896395*x^6*y^2*z^2 -33.65896395*y^6*z^2*x^2 +0.5749637458*x^8*y -0.5749637458*x^8*y^2 +0.2347202588*x*z^3 +3.640421878*x^6 +25.36672152*x^4*z^3 +25.36672152*y^3*z^4 +0.4659238756*z^2 -0.3389589536*z -120.2077842*x*y^2*z^2 +100.9768918*x*z^5*y +30.54472579*x^5*y^3 +16.91336805*x^6*z^3 -33.65896395*x^2*z^6*y^2 +33.65896395*x^6*y*z^2 +201.9537837*x^3*y^3*z^2 +2.567983615*x*y +18.5125231*x^2*y^2 -15.76344056*y^2*z^7 +5.090787632*x^6*y^4 -3.940860141*y*z^8 +100.9768918*y^5*z^2*x^2 +0.04360832264*x^4*y^4 +35.62606345*x^5*z^2 +117.9808071*x^3*y*z^2 -17.00391526*x*z^3*y +35.62606345*y^2*z^5 -4.774233544*y^6*z^2 -42.37063678*y^4*z^3 +0.5749637458*x*z^8 +3.940860141*y^8*x^2 -2.299854983*x^7*y -3.380386036*z^4 +0.2347202588*y^3*z -25.60810291*y^3*z^2 +16.91336805*x^3*y^6 +3.640421878*y^6 +33.83339724*x^3*z^3 +33.83339724*y^3*z^3 +19.60032929*y^4*x +8.917960033*x^2*z^4 -10.25601137*y*z^5 -3.716625642*y*z^3 -5.892963096*y^2*z -5.892963096*x^2*y +33.65896395*y^6*z^2*x +100.9768918*x^5*y^2*z^2 +44.38789864*y*z*x^2 +44.38789864*y*z^2*x -3.39496861*z^7 -10.18157526*x^3*z^6 +100.9768918*x^4*y*z^3 -5.046246117*x*y^2 -10.25601137*y^5*x -14.9720102*y*z^6 +15.76344056*y*z^7 +0.8487421525*z^8 -75.64545224*x*z^4*y -75.64545224*x^4*y*z +25.15700632*y^4*z*x^2 +18.5125231*y^2*z^2 -14.9720102*y^6*x -8.456684027*y^4*z^6 +2.567983615*y*z +15.76344056*x^7*z -218.9576989*y^3*z^2*x^2 +25.33143961*y^2*z^4*x^2 -218.9576989*y^2*z^3*x^2 -17.00391526*y^3*z*x +117.9808071*y^3*z*x^2 -201.9537837*y^3*z^3*x -50.74010416*x^5*z^3 -2.299854983*y^7*z",
  /* Steiner relative */  "2-8*y*z*x-2*z-2*x-10*y+8*y^2-16*x*y^2-16*x^2*y+16*x^2*y^2+20*x*y-16*z^2*x^2+16*x^2*z+16*x*z^2+20*y*z-16*y*z^2-16*y^2*z+16*y^2*z^2-12*x*z",
  /* Mitre            */  "4-8*z-16*x-12*y+28*y^2-16*x*y^2-16*x^2*y+16*x^2*y^2+16*x*y+16*z^2*x^2-16*x^2*z-16*x*z^2+16*y*z-16*y*z^2-16*y^2*z+16*y^2*z^2+16*x*z+8*z^2+16*y^4+16*x^4-32*x^3+32*x^2-32*y^3",
  /* Glob tear        */  "16*x^5-32*x^4+24*x^3-8*x^2+x-2-4*y^2+4*y-4*z^2+4*z",
  /* Piri tear        */  "x^4-1*x^3+y^2-1*y+.5+z^2-1*z",
  /* Gumdrop torus    */  "-1792*z-2048*y*z^2-2368*x-1792*y+2048*y*z-2048*x^3+1024*x^4-2048*y^3-2048*z^3+1024*y^4+1024*z^4+4352*x*y+3392*x^2+2816*y^2+2816*z^2+641-4352*x*y^2+4352*x*z-4352*x*z^2-4352*x^2*z+4352*x^2*y^2+2048*y^2*z^2-4352*x^2*y-2048*y^2*z+4352*z^2*x^2",
  /* Bretzel          */  "-1*z-4.5*x-2.53125*y+7.59375*y^2-18*x*y^2+18*x*y+72*x^4*y^2-72*x^4*y-144*x^3*y^2+144*x^3*y+z^2+5.0625*y^4+674*x^4+.55640625-196*x^3+38.5*x^2-1408*x^5+256*x^8-1024*x^7+1664*x^6+90*x^2*y^2-90*x^2*y-10.125*y^3",
  /* Blob             */  "9*x^2-8.1*x+4.895+9*y^2-8.1*y+9*z^2-9*z+sin(12*x-5.4)+sin(12*y-5.4)+sin(12*z-6)",
  /* Bifolia          */  "4-16*z-16*x+4*y^2+8*x^2*y^2+32*z^2*x^2-32*x^2*z-8*x*y^2-32*x*z^2+32*x*z+8*y^2*z^2-8*y^2*z+32*z^2+y^4+16*z^4+16*x^4-32*x^3+32*x^2-32*z^3-12*x^2*y+12*x*y-6*y-12*y*z^2+12*y*z",
  /* Lemniscate       */  "16*x^4-32*x^3+20*x^2-4*x+2+4*y^2-4*y+4*z^2-4*z",
  /* Planes           */  "x*y",
  /* Cylinders        */  "((4*x-2)^2+(4*y-2)^2)^4+((4*x-2)^2+(4*z-2)^2)^4-((4*x-2)^2+(4*y-2)^2)^4*((4*x-2)^2+(4*z-2)^2)^4",
  /* Torus            */  "(1.4-sqrt((6*x-3)^2+(6*y-3)^2))^2+(6*z-3)^2-1.44"
} ;
