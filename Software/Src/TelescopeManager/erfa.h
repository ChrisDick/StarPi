#ifndef ERFA_H
#define ERFA_H

#include <math.h>

/*
**  - - - - - - -
**   e r f a . h
**  - - - - - - -
**
**  Prototype function declarations and macros for erfa library.
**
*/

/* Star-independent astrometry parameters */
typedef struct {
   double pmt;        /* PM time interval (SSB, Julian years) */
   double eb[3];      /* SSB to observer (vector, au) */
   double eh[3];      /* Sun to observer (unit vector) */
   double em;         /* distance from Sun to observer (au) */
   double v[3];       /* barycentric observer velocity (vector, c) */
   double bm1;        /* sqrt(1-|v|^2): reciprocal of Lorenz factor */
   double bpn[3][3];  /* bias-precession-nutation matrix */
   double along;      /* longitude + s' + dERA(DUT) (radians) */
   double phi;        /* geodetic latitude (radians) */
   double xpl;        /* polar motion xp wrt local meridian (radians) */
   double ypl;        /* polar motion yp wrt local meridian (radians) */
   double sphi;       /* sine of geodetic latitude */
   double cphi;       /* cosine of geodetic latitude */
   double diurab;     /* magnitude of diurnal aberration vector */
   double eral;       /* "local" Earth rotation angle (radians) */
   double refa;       /* refraction constant A (radians) */
   double refb;       /* refraction constant B (radians) */
} eraASTROM;
/* (Vectors eb, eh, em and v are all with respect to BCRS axes.) */

/* Body parameters for light deflection */
typedef struct {
   double bm;         /* mass of the body (solar masses) */
   double dl;         /* deflection limiter (radians^2/2) */
   double pv[2][3];   /* barycentric PV of the body (au, au/day) */
} eraLDBODY;

/* Pi */
#define ERFA_DPI (3.141592653589793238462643)

/* 2Pi */
#define ERFA_D2PI (6.283185307179586476925287)

/* Radians to degrees */
#define ERFA_DR2D (57.29577951308232087679815)

/* Degrees to radians */
#define ERFA_DD2R (1.745329251994329576923691e-2)

/* Radians to arcseconds */
#define ERFA_DR2AS (206264.8062470963551564734)

/* Arcseconds to radians */
#define ERFA_DAS2R (4.848136811095359935899141e-6)

/* Seconds of time to radians */
#define ERFA_DS2R (7.272205216643039903848712e-5)

/* Arcseconds in a full circle */
#define ERFA_TURNAS (1296000.0)

/* Milliarcseconds to radians */
#define ERFA_DMAS2R (ERFA_DAS2R / 1e3)

/* Length of tropical year B1900 (days) */
#define ERFA_DTY (365.242198781)

/* Seconds per day. */
#define ERFA_DAYSEC (86400.0)

/* Days per Julian year */
#define ERFA_DJY (365.25)

/* Days per Julian century */
#define ERFA_DJC (36525.0)

/* Days per Julian millennium */
#define ERFA_DJM (365250.0)

/* Reference epoch (J2000.0), Julian Date */
#define ERFA_DJ00 (2451545.0)

/* Julian Date of Modified Julian Date zero */
#define ERFA_DJM0 (2400000.5)

/* Reference epoch (J2000.0), Modified Julian Date */
#define ERFA_DJM00 (51544.5)

/* 1977 Jan 1.0 as MJD */
#define ERFA_DJM77 (43144.0)

/* TT minus TAI (s) */
#define ERFA_TTMTAI (32.184)

/* Astronomical unit (m, IAU 2012) */
#define ERFA_DAU (149597870.7e3)

/* Speed of light (m/s) */
#define ERFA_CMPS 299792458.0

/* Light time for 1 au (s) */
#define ERFA_AULT (ERFA_DAU/ERFA_CMPS)

/* Speed of light (au per day) */
#define ERFA_DC (ERFA_DAYSEC/ERFA_AULT)

/* L_G = 1 - d(TT)/d(TCG) */
#define ERFA_ELG (6.969290134e-10)

/* L_B = 1 - d(TDB)/d(TCB), and TDB (s) at TAI 1977/1/1.0 */
#define ERFA_ELB (1.550519768e-8)
#define ERFA_TDB0 (-6.55e-5)

/* Schwarzschild radius of the Sun (au) */
/* = 2 * 1.32712440041e20 / (2.99792458e8)^2 / 1.49597870700e11 */
#define ERFA_SRS 1.97412574336e-8

/* ERFA_DINT(A) - truncate to nearest whole number towards zero (double) */
#define ERFA_DINT(A) ((A)<0.0?ceil(A):floor(A))

/* ERFA_DNINT(A) - round to nearest whole number (double) */
#define ERFA_DNINT(A) ((A)<0.0?ceil((A)-0.5):floor((A)+0.5))

/* ERFA_DSIGN(A,B) - magnitude of A with sign of B (double) */
#define ERFA_DSIGN(A,B) ((B)<0.0?-fabs(A):fabs(A))

/* max(A,B) - larger (most +ve) of two numbers (generic) */
#define ERFA_GMAX(A,B) (((A)>(B))?(A):(B))

/* min(A,B) - smaller (least +ve) of two numbers (generic) */
#define ERFA_GMIN(A,B) (((A)<(B))?(A):(B))

/* Reference ellipsoids */
#define ERFA_WGS84 1
#define ERFA_GRS80 2
#define ERFA_WGS72 3

class erfa
{
    public:
    /* constructor */
    erfa( void );
/* Astronomy/Calendars */
int Cal2jd(int iy, int im, int id, double *djm0, double *djm);
double Epb(double dj1, double dj2);
void Epb2jd(double epb, double *djm0, double *djm);
double Epj(double dj1, double dj2);
void Epj2jd(double epj, double *djm0, double *djm);
int Jd2cal(double dj1, double dj2,
                     int *iy, int *im, int *id, double *fd);
int Jdcalf(int ndp, double dj1, double dj2, int iymdf[4]);

/* Astronomy/Astrometry */
void Ab(double pnat[3], double v[3], double s, double bm1,
           double ppr[3]);
void Apcg(double date1, double date2,
             double ebpv[2][3], double ehp[3],
             eraASTROM *astrom);
void Apcg13(double date1, double date2, eraASTROM *astrom);
void Apci(double date1, double date2,
             double ebpv[2][3], double ehp[3],
             double x, double y, double s,
             eraASTROM *astrom);
void Apci13(double date1, double date2,
             eraASTROM *astrom, double *eo);
void Apco(double date1, double date2,
             double ebpv[2][3], double ehp[3],
             double x, double y, double s, double theta,
             double elong, double phi, double hm,
             double xp, double yp, double sp,
             double refa, double refb,
             eraASTROM *astrom);
int Apco13(double utc1, double utc2, double dut1,
              double elong, double phi, double hm, double xp, double yp,
              double phpa, double tc, double rh, double wl,
              eraASTROM *astrom, double *eo);
void Apcs(double date1, double date2, double pv[2][3],
             double ebpv[2][3], double ehp[3],
             eraASTROM *astrom);
void Apcs13(double date1, double date2, double pv[2][3],
               eraASTROM *astrom);
void Aper(double theta, eraASTROM *astrom);
void Aper13(double ut11, double ut12, eraASTROM *astrom);
void Apio(double sp, double theta,
             double elong, double phi, double hm, double xp, double yp,
             double refa, double refb,
             eraASTROM *astrom);
int Apio13(double utc1, double utc2, double dut1,
              double elong, double phi, double hm, double xp, double yp,
              double phpa, double tc, double rh, double wl,
              eraASTROM *astrom);
void Atci13(double rc, double dc,
               double pr, double pd, double px, double rv,
               double date1, double date2,
               double *ri, double *di, double *eo);
void Atciq(double rc, double dc, double pr, double pd,
              double px, double rv, eraASTROM *astrom,
              double *ri, double *di);
void Atciqn(double rc, double dc, double pr, double pd,
               double px, double rv, eraASTROM *astrom,
               int n, eraLDBODY b[], double *ri, double *di);
void Atciqz(double rc, double dc, eraASTROM *astrom,
               double *ri, double *di);
int Atco13(double rc, double dc,
              double pr, double pd, double px, double rv,
              double utc1, double utc2, double dut1,
              double elong, double phi, double hm, double xp, double yp,
              double phpa, double tc, double rh, double wl,
              double *aob, double *zob, double *hob,
              double *dob, double *rob, double *eo);
void Atic13(double ri, double di,
               double date1, double date2,
               double *rc, double *dc, double *eo);
void Aticq(double ri, double di, eraASTROM *astrom,
              double *rc, double *dc);
void Aticqn(double ri, double di, eraASTROM *astrom,
               int n, eraLDBODY b[], double *rc, double *dc);
int Atio13(double ri, double di,
              double utc1, double utc2, double dut1,
              double elong, double phi, double hm, double xp, double yp,
              double phpa, double tc, double rh, double wl,
              double *aob, double *zob, double *hob,
              double *dob, double *rob);
void Atioq(double ri, double di, eraASTROM *astrom,
              double *aob, double *zob,
              double *hob, double *dob, double *rob);
int Atoc13(const char *type, double ob1, double ob2,
              double utc1, double utc2, double dut1,
              double elong, double phi, double hm, double xp, double yp,
              double phpa, double tc, double rh, double wl,
              double *rc, double *dc);
int Atoi13(const char *type, double ob1, double ob2,
              double utc1, double utc2, double dut1,
              double elong, double phi, double hm, double xp, double yp,
              double phpa, double tc, double rh, double wl,
              double *ri, double *di);
void Atoiq(const char *type,
              double ob1, double ob2, eraASTROM *astrom,
              double *ri, double *di);
void Ld(double bm, double p[3], double q[3], double e[3],
          double em, double dlim, double p1[3]);
void Ldn(int n, eraLDBODY b[], double ob[3], double sc[3],
            double sn[3]);
void Ldsun(double p[3], double e[3], double em, double p1[3]);
void Pmpx(double rc, double dc, double pr, double pd,
             double px, double rv, double pmt, double pob[3],
             double pco[3]);
int Pmsafe(double ra1, double dec1, double pmr1, double pmd1,
              double px1, double rv1,
              double ep1a, double ep1b, double ep2a, double ep2b,
              double *ra2, double *dec2, double *pmr2, double *pmd2,
              double *px2, double *rv2);
void Pvtob(double elong, double phi, double height, double xp,
              double yp, double sp, double theta, double pv[2][3]);
void Refco(double phpa, double tc, double rh, double wl,
              double *refa, double *refb);

/* Astronomy/Ephemerides */
int Epv00(double date1, double date2,
             double pvh[2][3], double pvb[2][3]);
int Plan94(double date1, double date2, int np, double pv[2][3]);

/* Astronomy/FundamentalArgs */
double Fad03(double t);
double Fae03(double t);
double Faf03(double t);
double Faju03(double t);
double Fal03(double t);
double Falp03(double t);
double Fama03(double t);
double Fame03(double t);
double Fane03(double t);
double Faom03(double t);
double Fapa03(double t);
double Fasa03(double t);
double Faur03(double t);
double Fave03(double t);

/* Astronomy/PrecNutPolar */
void Bi00(double *dpsibi, double *depsbi, double *dra);
void Bp00(double date1, double date2,
             double rb[3][3], double rp[3][3], double rbp[3][3]);
void Bp06(double date1, double date2,
             double rb[3][3], double rp[3][3], double rbp[3][3]);
void Bpn2xy(double rbpn[3][3], double *x, double *y);
void C2i00a(double date1, double date2, double rc2i[3][3]);
void C2i00b(double date1, double date2, double rc2i[3][3]);
void C2i06a(double date1, double date2, double rc2i[3][3]);
void C2ibpn(double date1, double date2, double rbpn[3][3],
               double rc2i[3][3]);
void C2ixy(double date1, double date2, double x, double y,
              double rc2i[3][3]);
void C2ixys(double x, double y, double s, double rc2i[3][3]);
void C2t00a(double tta, double ttb, double uta, double utb,
               double xp, double yp, double rc2t[3][3]);
void C2t00b(double tta, double ttb, double uta, double utb,
               double xp, double yp, double rc2t[3][3]);
void C2t06a(double tta, double ttb, double uta, double utb,
               double xp, double yp, double rc2t[3][3]);
void C2tcio(double rc2i[3][3], double era, double rpom[3][3],
               double rc2t[3][3]);
void C2teqx(double rbpn[3][3], double gst, double rpom[3][3],
               double rc2t[3][3]);
void C2tpe(double tta, double ttb, double uta, double utb,
              double dpsi, double deps, double xp, double yp,
              double rc2t[3][3]);
void C2txy(double tta, double ttb, double uta, double utb,
              double x, double y, double xp, double yp,
              double rc2t[3][3]);
double Eo06a(double date1, double date2);
double Eors(double rnpb[3][3], double s);
void Fw2m(double gamb, double phib, double psi, double eps,
             double r[3][3]);
void Fw2xy(double gamb, double phib, double psi, double eps,
              double *x, double *y);
void Ltp(double epj, double rp[3][3]);
void Ltpb(double epj, double rpb[3][3]);
void Ltpecl(double epj, double vec[3]);
void Ltpequ(double epj, double veq[3]);
void Num00a(double date1, double date2, double rmatn[3][3]);
void Num00b(double date1, double date2, double rmatn[3][3]);
void Num06a(double date1, double date2, double rmatn[3][3]);
void Numat(double epsa, double dpsi, double deps, double rmatn[3][3]);
void Nut00a(double date1, double date2, double *dpsi, double *deps);
void Nut00b(double date1, double date2, double *dpsi, double *deps);
void Nut06a(double date1, double date2, double *dpsi, double *deps);
void Nut80(double date1, double date2, double *dpsi, double *deps);
void Nutm80(double date1, double date2, double rmatn[3][3]);
double Obl06(double date1, double date2);
double Obl80(double date1, double date2);
void P06e(double date1, double date2,
             double *eps0, double *psia, double *oma, double *bpa,
             double *bqa, double *pia, double *bpia,
             double *epsa, double *chia, double *za, double *zetaa,
             double *thetaa, double *pa,
             double *gam, double *phi, double *psi);
void Pb06(double date1, double date2,
             double *bzeta, double *bz, double *btheta);
void Pfw06(double date1, double date2,
              double *gamb, double *phib, double *psib, double *epsa);
void Pmat00(double date1, double date2, double rbp[3][3]);
void Pmat06(double date1, double date2, double rbp[3][3]);
void Pmat76(double date1, double date2, double rmatp[3][3]);
void Pn00(double date1, double date2, double dpsi, double deps,
             double *epsa,
             double rb[3][3], double rp[3][3], double rbp[3][3],
             double rn[3][3], double rbpn[3][3]);
void Pn00a(double date1, double date2,
              double *dpsi, double *deps, double *epsa,
              double rb[3][3], double rp[3][3], double rbp[3][3],
              double rn[3][3], double rbpn[3][3]);
void Pn00b(double date1, double date2,
              double *dpsi, double *deps, double *epsa,
              double rb[3][3], double rp[3][3], double rbp[3][3],
              double rn[3][3], double rbpn[3][3]);
void Pn06(double date1, double date2, double dpsi, double deps,
             double *epsa,
             double rb[3][3], double rp[3][3], double rbp[3][3],
             double rn[3][3], double rbpn[3][3]);
void Pn06a(double date1, double date2,
              double *dpsi, double *deps, double *epsa,
              double rb[3][3], double rp[3][3], double rbp[3][3],
              double rn[3][3], double rbpn[3][3]);
void Pnm00a(double date1, double date2, double rbpn[3][3]);
void Pnm00b(double date1, double date2, double rbpn[3][3]);
void Pnm06a(double date1, double date2, double rnpb[3][3]);
void Pnm80(double date1, double date2, double rmatpn[3][3]);
void Pom00(double xp, double yp, double sp, double rpom[3][3]);
void Pr00(double date1, double date2,
             double *dpsipr, double *depspr);
void Prec76(double date01, double date02,
               double date11, double date12,
               double *zeta, double *z, double *theta);
double S00(double date1, double date2, double x, double y);
double S00a(double date1, double date2);
double S00b(double date1, double date2);
double S06(double date1, double date2, double x, double y);
double S06a(double date1, double date2);
double Sp00(double date1, double date2);
void Xy06(double date1, double date2, double *x, double *y);
void Xys00a(double date1, double date2,
               double *x, double *y, double *s);
void Xys00b(double date1, double date2,
               double *x, double *y, double *s);
void Xys06a(double date1, double date2,
               double *x, double *y, double *s);

/* Astronomy/RotationAndTime */
double Ee00(double date1, double date2, double epsa, double dpsi);
double Ee00a(double date1, double date2);
double Ee00b(double date1, double date2);
double Ee06a(double date1, double date2);
double Eect00(double date1, double date2);
double Eqeq94(double date1, double date2);
double Era00(double dj1, double dj2);
double Gmst00(double uta, double utb, double tta, double ttb);
double Gmst06(double uta, double utb, double tta, double ttb);
double Gmst82(double dj1, double dj2);
double Gst00a(double uta, double utb, double tta, double ttb);
double Gst00b(double uta, double utb);
double Gst06(double uta, double utb, double tta, double ttb,
                double rnpb[3][3]);
double Gst06a(double uta, double utb, double tta, double ttb);
double Gst94(double uta, double utb);

/* Astronomy/SpaceMotion */
int Pvstar(double pv[2][3], double *ra, double *dec,
              double *pmr, double *pmd, double *px, double *rv);
int Starpv(double ra, double dec,
              double pmr, double pmd, double px, double rv,
              double pv[2][3]);

/* Astronomy/StarCatalogs */
void Fk52h(double r5, double d5,
              double dr5, double dd5, double px5, double rv5,
              double *rh, double *dh,
              double *drh, double *ddh, double *pxh, double *rvh);
void Fk5hip(double r5h[3][3], double s5h[3]);
void Fk5hz(double r5, double d5, double date1, double date2,
              double *rh, double *dh);
void H2fk5(double rh, double dh,
              double drh, double ddh, double pxh, double rvh,
              double *r5, double *d5,
              double *dr5, double *dd5, double *px5, double *rv5);
void Hfk5z(double rh, double dh, double date1, double date2,
              double *r5, double *d5, double *dr5, double *dd5);
int Starpm(double ra1, double dec1,
              double pmr1, double pmd1, double px1, double rv1,
              double ep1a, double ep1b, double ep2a, double ep2b,
              double *ra2, double *dec2,
              double *pmr2, double *pmd2, double *px2, double *rv2);

/* Astronomy/EclipticCoordinates */
void Eceq06(double date1, double date2, double dl, double db,
               double *dr, double *dd);
void Ecm06(double date1, double date2, double rm[3][3]);
void Eqec06(double date1, double date2, double dr, double dd,
               double *dl, double *db);
void Lteceq(double epj, double dl, double db, double *dr, double *dd);
void Ltecm(double epj, double rm[3][3]);
void Lteqec(double epj, double dr, double dd, double *dl, double *db);

/* Astronomy/GalacticCoordinates */
void G2icrs(double dl, double db, double *dr, double *dd);
void Icrs2g(double dr, double dd, double *dl, double *db);

/* Astronomy/GeodeticGeocentric */
int Eform(int n, double *a, double *f);
int Gc2gd(int n, double xyz[3],
             double *elong, double *phi, double *height);
int Gc2gde(double a, double f, double xyz[3],
              double *elong, double *phi, double *height);
int Gd2gc(int n, double elong, double phi, double height,
             double xyz[3]);
int Gd2gce(double a, double f,
              double elong, double phi, double height, double xyz[3]);

/* Astronomy/Timescales */
int D2dtf(const char *scale, int ndp, double d1, double d2,
             int *iy, int *im, int *id, int ihmsf[4]);
int Dat(int iy, int im, int id, double fd, double *deltat);
double Dtdb(double date1, double date2,
               double ut, double elong, double u, double v);
int Dtf2d(const char *scale, int iy, int im, int id,
             int ihr, int imn, double sec, double *d1, double *d2);
int Taitt(double tai1, double tai2, double *tt1, double *tt2);
int Taiut1(double tai1, double tai2, double dta,
              double *ut11, double *ut12);
int Taiutc(double tai1, double tai2, double *utc1, double *utc2);
int Tcbtdb(double tcb1, double tcb2, double *tdb1, double *tdb2);
int Tcgtt(double tcg1, double tcg2, double *tt1, double *tt2);
int Tdbtcb(double tdb1, double tdb2, double *tcb1, double *tcb2);
int Tdbtt(double tdb1, double tdb2, double dtr,
             double *tt1, double *tt2);
int Tttai(double tt1, double tt2, double *tai1, double *tai2);
int Tttcg(double tt1, double tt2, double *tcg1, double *tcg2);
int Tttdb(double tt1, double tt2, double dtr,
             double *tdb1, double *tdb2);
int Ttut1(double tt1, double tt2, double dt,
             double *ut11, double *ut12);
int Ut1tai(double ut11, double ut12, double dta,
              double *tai1, double *tai2);
int Ut1tt(double ut11, double ut12, double dt,
             double *tt1, double *tt2);
int Ut1utc(double ut11, double ut12, double dut1,
              double *utc1, double *utc2);
int Utctai(double utc1, double utc2, double *tai1, double *tai2);
int Utcut1(double utc1, double utc2, double dut1,
              double *ut11, double *ut12);

/* VectorMatrix/AngleOps */
void A2af(int ndp, double angle, char *sign, int idmsf[4]);
void A2tf(int ndp, double angle, char *sign, int ihmsf[4]);
int Af2a(char s, int ideg, int iamin, double asec, double *rad);
double Anp(double a);
double Anpm(double a);
void D2tf(int ndp, double days, char *sign, int ihmsf[4]);
int Tf2a(char s, int ihour, int imin, double sec, double *rad);
int Tf2d(char s, int ihour, int imin, double sec, double *days);

/* VectorMatrix/BuildRotations */
void Rx(double phi, double r[3][3]);
void Ry(double theta, double r[3][3]);
void Rz(double psi, double r[3][3]);

/* VectorMatrix/CopyExtendExtract */
void Cp(double p[3], double c[3]);
void Cpv(double pv[2][3], double c[2][3]);
void Cr(double r[3][3], double c[3][3]);
void P2pv(double p[3], double pv[2][3]);
void Pv2p(double pv[2][3], double p[3]);

/* VectorMatrix/Initialization */
void Ir(double r[3][3]);
void Zp(double p[3]);
void Zpv(double pv[2][3]);
void Zr(double r[3][3]);

/* VectorMatrix/MatrixOps */
void Rxr(double a[3][3], double b[3][3], double atb[3][3]);
void Tr(double r[3][3], double rt[3][3]);

/* VectorMatrix/MatrixVectorProducts */
void Rxp(double r[3][3], double p[3], double rp[3]);
void Rxpv(double r[3][3], double pv[2][3], double rpv[2][3]);
void Trxp(double r[3][3], double p[3], double trp[3]);
void Trxpv(double r[3][3], double pv[2][3], double trpv[2][3]);

/* VectorMatrix/RotationVectors */
void Rm2v(double r[3][3], double w[3]);
void Rv2m(double w[3], double r[3][3]);

/* VectorMatrix/SeparationAndAngle */
double Pap(double a[3], double b[3]);
double Pas(double al, double ap, double bl, double bp);
double Sepp(double a[3], double b[3]);
double Seps(double al, double ap, double bl, double bp);

/* VectorMatrix/SphericalCartesian */
void C2s(double p[3], double *theta, double *phi);
void P2s(double p[3], double *theta, double *phi, double *r);
void Pv2s(double pv[2][3],
             double *theta, double *phi, double *r,
             double *td, double *pd, double *rd);
void S2c(double theta, double phi, double c[3]);
void S2p(double theta, double phi, double r, double p[3]);
void S2pv(double theta, double phi, double r,
             double td, double pd, double rd,
             double pv[2][3]);

/* VectorMatrix/VectorOps */
double Pdp(double a[3], double b[3]);
double Pm(double p[3]);
void Pmp(double a[3], double b[3], double amb[3]);
void Pn(double p[3], double *r, double u[3]);
void Ppp(double a[3], double b[3], double apb[3]);
void Ppsp(double a[3], double s, double b[3], double apsb[3]);
void Pvdpv(double a[2][3], double b[2][3], double adb[2]);
void Pvm(double pv[2][3], double *r, double *s);
void Pvmpv(double a[2][3], double b[2][3], double amb[2][3]);
void Pvppv(double a[2][3], double b[2][3], double apb[2][3]);
void Pvu(double dt, double pv[2][3], double upv[2][3]);
void Pvup(double dt, double pv[2][3], double p[3]);
void Pvxpv(double a[2][3], double b[2][3], double axb[2][3]);
void Pxp(double a[3], double b[3], double axb[3]);
void S2xpv(double s1, double s2, double pv[2][3], double spv[2][3]);
void Sxp(double s, double p[3], double sp[3]);
void Sxpv(double s, double pv[2][3], double spv[2][3]);
};
#endif

/*----------------------------------------------------------------------
**  
**  
**  Copyright (C) 2013-2017, NumFOCUS Foundation.
**  All rights reserved.
**  
**  This library is derived, with permission, from the International
**  Astronomical Union's "Standards of Fundamental Astronomy" library,
**  available from http://www.iausofa.org.
**  
**  The ERFA version is intended to retain identical functionality to
**  the SOFA library, but made distinct through different function and
**  file names, as set out in the SOFA license conditions.  The SOFA
**  original has a role as a reference standard for the IAU and IERS,
**  and consequently redistribution is permitted only in its unaltered
**  state.  The ERFA version is not subject to this restriction and
**  therefore can be included in distributions which do not support the
**  concept of "read only" software.
**  
**  Although the intent is to replicate the SOFA API (other than
**  replacement of prefix names) and results (with the exception of
**  bugs;  any that are discovered will be fixed), SOFA is not
**  responsible for any errors found in this version of the library.
**  
**  If you wish to acknowledge the SOFA heritage, please acknowledge
**  that you are using a library derived from SOFA, rather than SOFA
**  itself.
**  
**  
**  TERMS AND CONDITIONS
**  
**  Redistribution and use in source and binary forms, with or without
**  modification, are permitted provided that the following conditions
**  are met:
**  
**  1 Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
**  
**  2 Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in
**    the documentation and/or other materials provided with the
**    distribution.
**  
**  3 Neither the name of the Standards Of Fundamental Astronomy Board,
**    the International Astronomical Union nor the names of its
**    contributors may be used to endorse or promote products derived
**    from this software without specific prior written permission.
**  
**  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
**  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
**  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
**  FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
**  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
**  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
**  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
**  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
**  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
**  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
**  POSSIBILITY OF SUCH DAMAGE.
**  
*/
