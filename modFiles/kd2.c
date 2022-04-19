/* Created by Language version: 7.7.0 */
/* VECTORIZED */
#define NRN_VECTORIZED 1
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "scoplib_ansi.h"
#undef PI
#define nil 0
#include "md1redef.h"
#include "section.h"
#include "nrniv_mf.h"
#include "md2redef.h"
 
#if METHOD3
extern int _method3;
#endif

#if !NRNGPU
#undef exp
#define exp hoc_Exp
extern double hoc_Exp(double);
#endif
 
#define nrn_init _nrn_init__kd2
#define _nrn_initial _nrn_initial__kd2
#define nrn_cur _nrn_cur__kd2
#define _nrn_current _nrn_current__kd2
#define nrn_jacob _nrn_jacob__kd2
#define nrn_state _nrn_state__kd2
#define _net_receive _net_receive__kd2 
#define states states__kd2 
 
#define _threadargscomma_ _p, _ppvar, _thread, _nt,
#define _threadargsprotocomma_ double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt,
#define _threadargs_ _p, _ppvar, _thread, _nt
#define _threadargsproto_ double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt
 	/*SUPPRESS 761*/
	/*SUPPRESS 762*/
	/*SUPPRESS 763*/
	/*SUPPRESS 765*/
	 extern double *getarg();
 /* Thread safe. No static _p or _ppvar. */
 
#define t _nt->_t
#define dt _nt->_dt
#define g1 _p[0]
#define g2 _p[1]
#define i _p[2]
#define m1 _p[3]
#define h1 _p[4]
#define m2 _p[5]
#define ek _p[6]
#define ik _p[7]
#define G _p[8]
#define Dm1 _p[9]
#define Dh1 _p[10]
#define Dm2 _p[11]
#define v _p[12]
#define _g _p[13]
#define _ion_ek	*_ppvar[0]._pval
#define _ion_ik	*_ppvar[1]._pval
#define _ion_dikdv	*_ppvar[2]._pval
 
#if MAC
#if !defined(v)
#define v _mlhv
#endif
#if !defined(h)
#define h _mlhh
#endif
#endif
 
#if defined(__cplusplus)
extern "C" {
#endif
 static int hoc_nrnpointerindex =  -1;
 static Datum* _extcall_thread;
 static Prop* _extcall_prop;
 /* external NEURON variables */
 /* declaration of user functions */
 static void _hoc_hinf1(void);
 static void _hoc_minf2(void);
 static void _hoc_minf1(void);
 static void _hoc_taum2(void);
 static void _hoc_tauh1(void);
 static void _hoc_taum1(void);
 static int _mechtype;
extern void _nrn_cacheloop_reg(int, int);
extern void hoc_register_prop_size(int, int, int);
extern void hoc_register_limits(int, HocParmLimits*);
extern void hoc_register_units(int, HocParmUnits*);
extern void nrn_promote(Prop*, int, int);
extern Memb_func* memb_func;
 
#define NMODL_TEXT 1
#if NMODL_TEXT
static const char* nmodl_file_text;
static const char* nmodl_filename;
extern void hoc_reg_nmodl_text(int, const char*);
extern void hoc_reg_nmodl_filename(int, const char*);
#endif

 extern void _nrn_setdata_reg(int, void(*)(Prop*));
 static void _setdata(Prop* _prop) {
 _extcall_prop = _prop;
 }
 static void _hoc_setdata() {
 Prop *_prop, *hoc_getdata_range(int);
 _prop = hoc_getdata_range(_mechtype);
   _setdata(_prop);
 hoc_retpushx(1.);
}
 /* connect user functions to hoc names */
 static VoidFunc hoc_intfunc[] = {
 "setdata_kd2", _hoc_setdata,
 "hinf1_kd2", _hoc_hinf1,
 "minf2_kd2", _hoc_minf2,
 "minf1_kd2", _hoc_minf1,
 "taum2_kd2", _hoc_taum2,
 "tauh1_kd2", _hoc_tauh1,
 "taum1_kd2", _hoc_taum1,
 0, 0
};
#define _f_tauh1 _f_tauh1_kd2
#define _f_taum2 _f_taum2_kd2
#define _f_taum1 _f_taum1_kd2
#define _f_hinf1 _f_hinf1_kd2
#define _f_minf2 _f_minf2_kd2
#define _f_minf1 _f_minf1_kd2
#define hinf1 hinf1_kd2
#define minf2 minf2_kd2
#define minf1 minf1_kd2
#define taum2 taum2_kd2
#define tauh1 tauh1_kd2
#define taum1 taum1_kd2
 extern double _f_tauh1( _threadargsprotocomma_ double );
 extern double _f_taum2( _threadargsprotocomma_ double );
 extern double _f_taum1( _threadargsprotocomma_ double );
 extern double _f_hinf1( _threadargsprotocomma_ double );
 extern double _f_minf2( _threadargsprotocomma_ double );
 extern double _f_minf1( _threadargsprotocomma_ double );
 extern double hinf1( _threadargsprotocomma_ double );
 extern double minf2( _threadargsprotocomma_ double );
 extern double minf1( _threadargsprotocomma_ double );
 extern double taum2( _threadargsprotocomma_ double );
 extern double tauh1( _threadargsprotocomma_ double );
 extern double taum1( _threadargsprotocomma_ double );
 
static void _check_minf1(double*, Datum*, Datum*, _NrnThread*); 
static void _check_minf2(double*, Datum*, Datum*, _NrnThread*); 
static void _check_hinf1(double*, Datum*, Datum*, _NrnThread*); 
static void _check_taum1(double*, Datum*, Datum*, _NrnThread*); 
static void _check_taum2(double*, Datum*, Datum*, _NrnThread*); 
static void _check_tauh1(double*, Datum*, Datum*, _NrnThread*); 
static void _check_table_thread(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, int _type) {
   _check_minf1(_p, _ppvar, _thread, _nt);
   _check_minf2(_p, _ppvar, _thread, _nt);
   _check_hinf1(_p, _ppvar, _thread, _nt);
   _check_taum1(_p, _ppvar, _thread, _nt);
   _check_taum2(_p, _ppvar, _thread, _nt);
   _check_tauh1(_p, _ppvar, _thread, _nt);
 }
 /* declare global and static user variables */
#define usetable usetable_kd2
 double usetable = 1;
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 "g2_kd2", 0, 1e+09,
 "g1_kd2", 0, 1e+09,
 "usetable_kd2", 0, 1,
 0,0,0
};
 static HocParmUnits _hoc_parm_units[] = {
 "g1_kd2", "siemens/cm2",
 "g2_kd2", "siemens/cm2",
 0,0
};
 static double delta_t = 0.01;
 static double h10 = 0;
 static double m20 = 0;
 static double m10 = 0;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 "usetable_kd2", &usetable_kd2,
 0,0
};
 static DoubVec hoc_vdoub[] = {
 0,0,0
};
 static double _sav_indep;
 static void nrn_alloc(Prop*);
static void  nrn_init(_NrnThread*, _Memb_list*, int);
static void nrn_state(_NrnThread*, _Memb_list*, int);
 static void nrn_cur(_NrnThread*, _Memb_list*, int);
static void  nrn_jacob(_NrnThread*, _Memb_list*, int);
 
static int _ode_count(int);
static void _ode_map(int, double**, double**, double*, Datum*, double*, int);
static void _ode_spec(_NrnThread*, _Memb_list*, int);
static void _ode_matsol(_NrnThread*, _Memb_list*, int);
 
#define _cvode_ieq _ppvar[3]._i
 static void _ode_matsol_instance1(_threadargsproto_);
 /* connect range variables in _p that hoc is supposed to know about */
 static const char *_mechanism[] = {
 "7.7.0",
"kd2",
 "g1_kd2",
 "g2_kd2",
 0,
 "i_kd2",
 0,
 "m1_kd2",
 "h1_kd2",
 "m2_kd2",
 0,
 0};
 static Symbol* _k_sym;
 
extern Prop* need_memb(Symbol*);

static void nrn_alloc(Prop* _prop) {
	Prop *prop_ion;
	double *_p; Datum *_ppvar;
 	_p = nrn_prop_data_alloc(_mechtype, 14, _prop);
 	/*initialize range parameters*/
 	g1 = 7.83e-07;
 	g2 = 3.356e-07;
 	_prop->param = _p;
 	_prop->param_size = 14;
 	_ppvar = nrn_prop_datum_alloc(_mechtype, 4, _prop);
 	_prop->dparam = _ppvar;
 	/*connect ionic variables to this model*/
 prop_ion = need_memb(_k_sym);
 nrn_promote(prop_ion, 0, 1);
 	_ppvar[0]._pval = &prop_ion->param[0]; /* ek */
 	_ppvar[1]._pval = &prop_ion->param[3]; /* ik */
 	_ppvar[2]._pval = &prop_ion->param[4]; /* _ion_dikdv */
 
}
 static void _initlists();
  /* some states have an absolute tolerance */
 static Symbol** _atollist;
 static HocStateTolerance _hoc_state_tol[] = {
 0,0
};
 static void _update_ion_pointer(Datum*);
 extern Symbol* hoc_lookup(const char*);
extern void _nrn_thread_reg(int, int, void(*)(Datum*));
extern void _nrn_thread_table_reg(int, void(*)(double*, Datum*, Datum*, _NrnThread*, int));
extern void hoc_register_tolerance(int, HocStateTolerance*, Symbol***);
extern void _cvode_abstol( Symbol**, double*, int);

 void _kd2_reg() {
	int _vectorized = 1;
  _initlists();
 	ion_reg("k", -10000.);
 	_k_sym = hoc_lookup("k_ion");
 	register_mech(_mechanism, nrn_alloc,nrn_cur, nrn_jacob, nrn_state, nrn_init, hoc_nrnpointerindex, 1);
 _mechtype = nrn_get_mechtype(_mechanism[1]);
     _nrn_setdata_reg(_mechtype, _setdata);
     _nrn_thread_reg(_mechtype, 2, _update_ion_pointer);
     _nrn_thread_table_reg(_mechtype, _check_table_thread);
 #if NMODL_TEXT
  hoc_reg_nmodl_text(_mechtype, nmodl_file_text);
  hoc_reg_nmodl_filename(_mechtype, nmodl_filename);
#endif
  hoc_register_prop_size(_mechtype, 14, 4);
  hoc_register_dparam_semantics(_mechtype, 0, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "cvodeieq");
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 	hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 kd2 C:/Users/ddopp/source/repos/CGProjectpub/modFiles/kd2.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
 static double *_t_minf1;
 static double *_t_minf2;
 static double *_t_hinf1;
 static double *_t_taum1;
 static double *_t_taum2;
 static double *_t_tauh1;
static int _reset;
static char *modelname = "";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
 
static int _ode_spec1(_threadargsproto_);
/*static int _ode_matsol1(_threadargsproto_);*/
 static double _n_tauh1(_threadargsprotocomma_ double _lv);
 static double _n_taum2(_threadargsprotocomma_ double _lv);
 static double _n_taum1(_threadargsprotocomma_ double _lv);
 static double _n_hinf1(_threadargsprotocomma_ double _lv);
 static double _n_minf2(_threadargsprotocomma_ double _lv);
 static double _n_minf1(_threadargsprotocomma_ double _lv);
 static int _slist1[3], _dlist1[3];
 static int states(_threadargsproto_);
 
/*CVODE*/
 static int _ode_spec1 (double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {int _reset = 0; {
   Dm1 = ( minf1 ( _threadargscomma_ v ) - m1 ) / taum1 ( _threadargscomma_ v ) ;
   Dh1 = ( hinf1 ( _threadargscomma_ v ) - h1 ) / tauh1 ( _threadargscomma_ v ) ;
   Dm2 = ( minf2 ( _threadargscomma_ v ) - m2 ) / taum2 ( _threadargscomma_ v ) ;
   }
 return _reset;
}
 static int _ode_matsol1 (double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {
 Dm1 = Dm1  / (1. - dt*( ( ( ( - 1.0 ) ) ) / taum1 ( _threadargscomma_ v ) )) ;
 Dh1 = Dh1  / (1. - dt*( ( ( ( - 1.0 ) ) ) / tauh1 ( _threadargscomma_ v ) )) ;
 Dm2 = Dm2  / (1. - dt*( ( ( ( - 1.0 ) ) ) / taum2 ( _threadargscomma_ v ) )) ;
  return 0;
}
 /*END CVODE*/
 static int states (double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) { {
    m1 = m1 + (1. - exp(dt*(( ( ( - 1.0 ) ) ) / taum1 ( _threadargscomma_ v ))))*(- ( ( ( minf1 ( _threadargscomma_ v ) ) ) / taum1 ( _threadargscomma_ v ) ) / ( ( ( ( - 1.0 ) ) ) / taum1 ( _threadargscomma_ v ) ) - m1) ;
    h1 = h1 + (1. - exp(dt*(( ( ( - 1.0 ) ) ) / tauh1 ( _threadargscomma_ v ))))*(- ( ( ( hinf1 ( _threadargscomma_ v ) ) ) / tauh1 ( _threadargscomma_ v ) ) / ( ( ( ( - 1.0 ) ) ) / tauh1 ( _threadargscomma_ v ) ) - h1) ;
    m2 = m2 + (1. - exp(dt*(( ( ( - 1.0 ) ) ) / taum2 ( _threadargscomma_ v ))))*(- ( ( ( minf2 ( _threadargscomma_ v ) ) ) / taum2 ( _threadargscomma_ v ) ) / ( ( ( ( - 1.0 ) ) ) / taum2 ( _threadargscomma_ v ) ) - m2) ;
   }
  return 0;
}
 static double _mfac_minf1, _tmin_minf1;
  static void _check_minf1(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {
  static int _maktable=1; int _i, _j, _ix = 0;
  double _xi, _tmax;
  if (!usetable) {return;}
  if (_maktable) { double _x, _dx; _maktable=0;
   _tmin_minf1 =  - 150.0 ;
   _tmax =  150.0 ;
   _dx = (_tmax - _tmin_minf1)/500.; _mfac_minf1 = 1./_dx;
   for (_i=0, _x=_tmin_minf1; _i < 501; _x += _dx, _i++) {
    _t_minf1[_i] = _f_minf1(_p, _ppvar, _thread, _nt, _x);
   }
  }
 }

 double minf1(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _lv) { 
#if 0
_check_minf1(_p, _ppvar, _thread, _nt);
#endif
 return _n_minf1(_p, _ppvar, _thread, _nt, _lv);
 }

 static double _n_minf1(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _lv){ int _i, _j;
 double _xi, _theta;
 if (!usetable) {
 return _f_minf1(_p, _ppvar, _thread, _nt, _lv); 
}
 _xi = _mfac_minf1 * (_lv - _tmin_minf1);
 if (isnan(_xi)) {
  return _xi; }
 if (_xi <= 0.) {
 return _t_minf1[0];
 }
 if (_xi >= 500.) {
 return _t_minf1[500];
 }
 _i = (int) _xi;
 return _t_minf1[_i] + (_xi - (double)_i)*(_t_minf1[_i+1] - _t_minf1[_i]);
 }

 
double _f_minf1 ( _threadargsprotocomma_ double _lv ) {
   double _lminf1;
 _lminf1 = 1.0 / ( 1.0 + exp ( ( _lv + 24.19 ) / - 10.77 ) ) ;
   
return _lminf1;
 }
 
static void _hoc_minf1(void) {
  double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   if (_extcall_prop) {_p = _extcall_prop->param; _ppvar = _extcall_prop->dparam;}else{ _p = (double*)0; _ppvar = (Datum*)0; }
  _thread = _extcall_thread;
  _nt = nrn_threads;
 
#if 1
 _check_minf1(_p, _ppvar, _thread, _nt);
#endif
 _r =  minf1 ( _p, _ppvar, _thread, _nt, *getarg(1) );
 hoc_retpushx(_r);
}
 static double _mfac_minf2, _tmin_minf2;
  static void _check_minf2(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {
  static int _maktable=1; int _i, _j, _ix = 0;
  double _xi, _tmax;
  if (!usetable) {return;}
  if (_maktable) { double _x, _dx; _maktable=0;
   _tmin_minf2 =  - 150.0 ;
   _tmax =  150.0 ;
   _dx = (_tmax - _tmin_minf2)/500.; _mfac_minf2 = 1./_dx;
   for (_i=0, _x=_tmin_minf2; _i < 501; _x += _dx, _i++) {
    _t_minf2[_i] = _f_minf2(_p, _ppvar, _thread, _nt, _x);
   }
  }
 }

 double minf2(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _lv) { 
#if 0
_check_minf2(_p, _ppvar, _thread, _nt);
#endif
 return _n_minf2(_p, _ppvar, _thread, _nt, _lv);
 }

 static double _n_minf2(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _lv){ int _i, _j;
 double _xi, _theta;
 if (!usetable) {
 return _f_minf2(_p, _ppvar, _thread, _nt, _lv); 
}
 _xi = _mfac_minf2 * (_lv - _tmin_minf2);
 if (isnan(_xi)) {
  return _xi; }
 if (_xi <= 0.) {
 return _t_minf2[0];
 }
 if (_xi >= 500.) {
 return _t_minf2[500];
 }
 _i = (int) _xi;
 return _t_minf2[_i] + (_xi - (double)_i)*(_t_minf2[_i+1] - _t_minf2[_i]);
 }

 
double _f_minf2 ( _threadargsprotocomma_ double _lv ) {
   double _lminf2;
 _lminf2 = 1.0 / ( 1.0 + exp ( ( _lv + 23.32 ) / - 10.0 ) ) ;
   
return _lminf2;
 }
 
static void _hoc_minf2(void) {
  double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   if (_extcall_prop) {_p = _extcall_prop->param; _ppvar = _extcall_prop->dparam;}else{ _p = (double*)0; _ppvar = (Datum*)0; }
  _thread = _extcall_thread;
  _nt = nrn_threads;
 
#if 1
 _check_minf2(_p, _ppvar, _thread, _nt);
#endif
 _r =  minf2 ( _p, _ppvar, _thread, _nt, *getarg(1) );
 hoc_retpushx(_r);
}
 static double _mfac_hinf1, _tmin_hinf1;
  static void _check_hinf1(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {
  static int _maktable=1; int _i, _j, _ix = 0;
  double _xi, _tmax;
  if (!usetable) {return;}
  if (_maktable) { double _x, _dx; _maktable=0;
   _tmin_hinf1 =  - 150.0 ;
   _tmax =  150.0 ;
   _dx = (_tmax - _tmin_hinf1)/500.; _mfac_hinf1 = 1./_dx;
   for (_i=0, _x=_tmin_hinf1; _i < 501; _x += _dx, _i++) {
    _t_hinf1[_i] = _f_hinf1(_p, _ppvar, _thread, _nt, _x);
   }
  }
 }

 double hinf1(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _lv) { 
#if 0
_check_hinf1(_p, _ppvar, _thread, _nt);
#endif
 return _n_hinf1(_p, _ppvar, _thread, _nt, _lv);
 }

 static double _n_hinf1(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _lv){ int _i, _j;
 double _xi, _theta;
 if (!usetable) {
 return _f_hinf1(_p, _ppvar, _thread, _nt, _lv); 
}
 _xi = _mfac_hinf1 * (_lv - _tmin_hinf1);
 if (isnan(_xi)) {
  return _xi; }
 if (_xi <= 0.) {
 return _t_hinf1[0];
 }
 if (_xi >= 500.) {
 return _t_hinf1[500];
 }
 _i = (int) _xi;
 return _t_hinf1[_i] + (_xi - (double)_i)*(_t_hinf1[_i+1] - _t_hinf1[_i]);
 }

 
double _f_hinf1 ( _threadargsprotocomma_ double _lv ) {
   double _lhinf1;
 _lhinf1 = 0.3 + ( ( 1.0 - 0.3 ) / ( 1.0 + exp ( ( _lv + 15.87 ) / 5.916 ) ) ) ;
   
return _lhinf1;
 }
 
static void _hoc_hinf1(void) {
  double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   if (_extcall_prop) {_p = _extcall_prop->param; _ppvar = _extcall_prop->dparam;}else{ _p = (double*)0; _ppvar = (Datum*)0; }
  _thread = _extcall_thread;
  _nt = nrn_threads;
 
#if 1
 _check_hinf1(_p, _ppvar, _thread, _nt);
#endif
 _r =  hinf1 ( _p, _ppvar, _thread, _nt, *getarg(1) );
 hoc_retpushx(_r);
}
 static double _mfac_taum1, _tmin_taum1;
  static void _check_taum1(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {
  static int _maktable=1; int _i, _j, _ix = 0;
  double _xi, _tmax;
  if (!usetable) {return;}
  if (_maktable) { double _x, _dx; _maktable=0;
   _tmin_taum1 =  - 150.0 ;
   _tmax =  150.0 ;
   _dx = (_tmax - _tmin_taum1)/500.; _mfac_taum1 = 1./_dx;
   for (_i=0, _x=_tmin_taum1; _i < 501; _x += _dx, _i++) {
    _t_taum1[_i] = _f_taum1(_p, _ppvar, _thread, _nt, _x);
   }
  }
 }

 double taum1(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _lv) { 
#if 0
_check_taum1(_p, _ppvar, _thread, _nt);
#endif
 return _n_taum1(_p, _ppvar, _thread, _nt, _lv);
 }

 static double _n_taum1(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _lv){ int _i, _j;
 double _xi, _theta;
 if (!usetable) {
 return _f_taum1(_p, _ppvar, _thread, _nt, _lv); 
}
 _xi = _mfac_taum1 * (_lv - _tmin_taum1);
 if (isnan(_xi)) {
  return _xi; }
 if (_xi <= 0.) {
 return _t_taum1[0];
 }
 if (_xi >= 500.) {
 return _t_taum1[500];
 }
 _i = (int) _xi;
 return _t_taum1[_i] + (_xi - (double)_i)*(_t_taum1[_i+1] - _t_taum1[_i]);
 }

 
double _f_taum1 ( _threadargsprotocomma_ double _lv ) {
   double _ltaum1;
 _ltaum1 = 25.049 + 25.0 / ( 1.0 + exp ( ( _lv + 25.84 ) / 6.252 ) ) ;
   
return _ltaum1;
 }
 
static void _hoc_taum1(void) {
  double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   if (_extcall_prop) {_p = _extcall_prop->param; _ppvar = _extcall_prop->dparam;}else{ _p = (double*)0; _ppvar = (Datum*)0; }
  _thread = _extcall_thread;
  _nt = nrn_threads;
 
#if 1
 _check_taum1(_p, _ppvar, _thread, _nt);
#endif
 _r =  taum1 ( _p, _ppvar, _thread, _nt, *getarg(1) );
 hoc_retpushx(_r);
}
 static double _mfac_taum2, _tmin_taum2;
  static void _check_taum2(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {
  static int _maktable=1; int _i, _j, _ix = 0;
  double _xi, _tmax;
  if (!usetable) {return;}
  if (_maktable) { double _x, _dx; _maktable=0;
   _tmin_taum2 =  - 150.0 ;
   _tmax =  150.0 ;
   _dx = (_tmax - _tmin_taum2)/500.; _mfac_taum2 = 1./_dx;
   for (_i=0, _x=_tmin_taum2; _i < 501; _x += _dx, _i++) {
    _t_taum2[_i] = _f_taum2(_p, _ppvar, _thread, _nt, _x);
   }
  }
 }

 double taum2(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _lv) { 
#if 0
_check_taum2(_p, _ppvar, _thread, _nt);
#endif
 return _n_taum2(_p, _ppvar, _thread, _nt, _lv);
 }

 static double _n_taum2(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _lv){ int _i, _j;
 double _xi, _theta;
 if (!usetable) {
 return _f_taum2(_p, _ppvar, _thread, _nt, _lv); 
}
 _xi = _mfac_taum2 * (_lv - _tmin_taum2);
 if (isnan(_xi)) {
  return _xi; }
 if (_xi <= 0.) {
 return _t_taum2[0];
 }
 if (_xi >= 500.) {
 return _t_taum2[500];
 }
 _i = (int) _xi;
 return _t_taum2[_i] + (_xi - (double)_i)*(_t_taum2[_i+1] - _t_taum2[_i]);
 }

 
double _f_taum2 ( _threadargsprotocomma_ double _lv ) {
   double _ltaum2;
 _ltaum2 = 100.0 + 550.0 / ( 1.0 + exp ( ( _lv + 15.0 ) / 12.46 ) ) ;
   
return _ltaum2;
 }
 
static void _hoc_taum2(void) {
  double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   if (_extcall_prop) {_p = _extcall_prop->param; _ppvar = _extcall_prop->dparam;}else{ _p = (double*)0; _ppvar = (Datum*)0; }
  _thread = _extcall_thread;
  _nt = nrn_threads;
 
#if 1
 _check_taum2(_p, _ppvar, _thread, _nt);
#endif
 _r =  taum2 ( _p, _ppvar, _thread, _nt, *getarg(1) );
 hoc_retpushx(_r);
}
 static double _mfac_tauh1, _tmin_tauh1;
  static void _check_tauh1(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {
  static int _maktable=1; int _i, _j, _ix = 0;
  double _xi, _tmax;
  if (!usetable) {return;}
  if (_maktable) { double _x, _dx; _maktable=0;
   _tmin_tauh1 =  - 150.0 ;
   _tmax =  150.0 ;
   _dx = (_tmax - _tmin_tauh1)/500.; _mfac_tauh1 = 1./_dx;
   for (_i=0, _x=_tmin_tauh1; _i < 501; _x += _dx, _i++) {
    _t_tauh1[_i] = _f_tauh1(_p, _ppvar, _thread, _nt, _x);
   }
  }
 }

 double tauh1(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _lv) { 
#if 0
_check_tauh1(_p, _ppvar, _thread, _nt);
#endif
 return _n_tauh1(_p, _ppvar, _thread, _nt, _lv);
 }

 static double _n_tauh1(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _lv){ int _i, _j;
 double _xi, _theta;
 if (!usetable) {
 return _f_tauh1(_p, _ppvar, _thread, _nt, _lv); 
}
 _xi = _mfac_tauh1 * (_lv - _tmin_tauh1);
 if (isnan(_xi)) {
  return _xi; }
 if (_xi <= 0.) {
 return _t_tauh1[0];
 }
 if (_xi >= 500.) {
 return _t_tauh1[500];
 }
 _i = (int) _xi;
 return _t_tauh1[_i] + (_xi - (double)_i)*(_t_tauh1[_i+1] - _t_tauh1[_i]);
 }

 
double _f_tauh1 ( _threadargsprotocomma_ double _lv ) {
   double _ltauh1;
 _ltauh1 = 550.0 + 954.9 / ( 1.0 + exp ( ( _lv + 10.8 ) / - 15.0 ) ) ;
   
return _ltauh1;
 }
 
static void _hoc_tauh1(void) {
  double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   if (_extcall_prop) {_p = _extcall_prop->param; _ppvar = _extcall_prop->dparam;}else{ _p = (double*)0; _ppvar = (Datum*)0; }
  _thread = _extcall_thread;
  _nt = nrn_threads;
 
#if 1
 _check_tauh1(_p, _ppvar, _thread, _nt);
#endif
 _r =  tauh1 ( _p, _ppvar, _thread, _nt, *getarg(1) );
 hoc_retpushx(_r);
}
 
static int _ode_count(int _type){ return 3;}
 
static void _ode_spec(_NrnThread* _nt, _Memb_list* _ml, int _type) {
   double* _p; Datum* _ppvar; Datum* _thread;
   Node* _nd; double _v; int _iml, _cntml;
  _cntml = _ml->_nodecount;
  _thread = _ml->_thread;
  for (_iml = 0; _iml < _cntml; ++_iml) {
    _p = _ml->_data[_iml]; _ppvar = _ml->_pdata[_iml];
    _nd = _ml->_nodelist[_iml];
    v = NODEV(_nd);
  ek = _ion_ek;
     _ode_spec1 (_p, _ppvar, _thread, _nt);
  }}
 
static void _ode_map(int _ieq, double** _pv, double** _pvdot, double* _pp, Datum* _ppd, double* _atol, int _type) { 
	double* _p; Datum* _ppvar;
 	int _i; _p = _pp; _ppvar = _ppd;
	_cvode_ieq = _ieq;
	for (_i=0; _i < 3; ++_i) {
		_pv[_i] = _pp + _slist1[_i];  _pvdot[_i] = _pp + _dlist1[_i];
		_cvode_abstol(_atollist, _atol, _i);
	}
 }
 
static void _ode_matsol_instance1(_threadargsproto_) {
 _ode_matsol1 (_p, _ppvar, _thread, _nt);
 }
 
static void _ode_matsol(_NrnThread* _nt, _Memb_list* _ml, int _type) {
   double* _p; Datum* _ppvar; Datum* _thread;
   Node* _nd; double _v; int _iml, _cntml;
  _cntml = _ml->_nodecount;
  _thread = _ml->_thread;
  for (_iml = 0; _iml < _cntml; ++_iml) {
    _p = _ml->_data[_iml]; _ppvar = _ml->_pdata[_iml];
    _nd = _ml->_nodelist[_iml];
    v = NODEV(_nd);
  ek = _ion_ek;
 _ode_matsol_instance1(_threadargs_);
 }}
 extern void nrn_update_ion_pointer(Symbol*, Datum*, int, int);
 static void _update_ion_pointer(Datum* _ppvar) {
   nrn_update_ion_pointer(_k_sym, _ppvar, 0, 0);
   nrn_update_ion_pointer(_k_sym, _ppvar, 1, 3);
   nrn_update_ion_pointer(_k_sym, _ppvar, 2, 4);
 }

static void initmodel(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {
  int _i; double _save;{
  h1 = h10;
  m2 = m20;
  m1 = m10;
 {
   m1 = minf1 ( _threadargscomma_ v ) ;
   h1 = hinf1 ( _threadargscomma_ v ) ;
   m2 = minf2 ( _threadargscomma_ v ) ;
   }
 
}
}

static void nrn_init(_NrnThread* _nt, _Memb_list* _ml, int _type){
double* _p; Datum* _ppvar; Datum* _thread;
Node *_nd; double _v; int* _ni; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
_thread = _ml->_thread;
for (_iml = 0; _iml < _cntml; ++_iml) {
 _p = _ml->_data[_iml]; _ppvar = _ml->_pdata[_iml];

#if 0
 _check_minf1(_p, _ppvar, _thread, _nt);
 _check_minf2(_p, _ppvar, _thread, _nt);
 _check_hinf1(_p, _ppvar, _thread, _nt);
 _check_taum1(_p, _ppvar, _thread, _nt);
 _check_taum2(_p, _ppvar, _thread, _nt);
 _check_tauh1(_p, _ppvar, _thread, _nt);
#endif
#if CACHEVEC
  if (use_cachevec) {
    _v = VEC_V(_ni[_iml]);
  }else
#endif
  {
    _nd = _ml->_nodelist[_iml];
    _v = NODEV(_nd);
  }
 v = _v;
  ek = _ion_ek;
 initmodel(_p, _ppvar, _thread, _nt);
 }
}

static double _nrn_current(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _v){double _current=0.;v=_v;{ {
   G = g1 * pow( m1 , 4.0 ) * h1 + g2 * pow( m2 , 4.0 ) ;
   i = G * ( v - ek ) ;
   ik = i ;
   }
 _current += ik;

} return _current;
}

static void nrn_cur(_NrnThread* _nt, _Memb_list* _ml, int _type) {
double* _p; Datum* _ppvar; Datum* _thread;
Node *_nd; int* _ni; double _rhs, _v; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
_thread = _ml->_thread;
for (_iml = 0; _iml < _cntml; ++_iml) {
 _p = _ml->_data[_iml]; _ppvar = _ml->_pdata[_iml];
#if CACHEVEC
  if (use_cachevec) {
    _v = VEC_V(_ni[_iml]);
  }else
#endif
  {
    _nd = _ml->_nodelist[_iml];
    _v = NODEV(_nd);
  }
  ek = _ion_ek;
 _g = _nrn_current(_p, _ppvar, _thread, _nt, _v + .001);
 	{ double _dik;
  _dik = ik;
 _rhs = _nrn_current(_p, _ppvar, _thread, _nt, _v);
  _ion_dikdv += (_dik - ik)/.001 ;
 	}
 _g = (_g - _rhs)/.001;
  _ion_ik += ik ;
#if CACHEVEC
  if (use_cachevec) {
	VEC_RHS(_ni[_iml]) -= _rhs;
  }else
#endif
  {
	NODERHS(_nd) -= _rhs;
  }
 
}
 
}

static void nrn_jacob(_NrnThread* _nt, _Memb_list* _ml, int _type) {
double* _p; Datum* _ppvar; Datum* _thread;
Node *_nd; int* _ni; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
_thread = _ml->_thread;
for (_iml = 0; _iml < _cntml; ++_iml) {
 _p = _ml->_data[_iml];
#if CACHEVEC
  if (use_cachevec) {
	VEC_D(_ni[_iml]) += _g;
  }else
#endif
  {
     _nd = _ml->_nodelist[_iml];
	NODED(_nd) += _g;
  }
 
}
 
}

static void nrn_state(_NrnThread* _nt, _Memb_list* _ml, int _type) {
double* _p; Datum* _ppvar; Datum* _thread;
Node *_nd; double _v = 0.0; int* _ni; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
_thread = _ml->_thread;
for (_iml = 0; _iml < _cntml; ++_iml) {
 _p = _ml->_data[_iml]; _ppvar = _ml->_pdata[_iml];
 _nd = _ml->_nodelist[_iml];
#if CACHEVEC
  if (use_cachevec) {
    _v = VEC_V(_ni[_iml]);
  }else
#endif
  {
    _nd = _ml->_nodelist[_iml];
    _v = NODEV(_nd);
  }
 v=_v;
{
  ek = _ion_ek;
 {   states(_p, _ppvar, _thread, _nt);
  } }}

}

static void terminal(){}

static void _initlists(){
 double _x; double* _p = &_x;
 int _i; static int _first = 1;
  if (!_first) return;
 _slist1[0] = &(m1) - _p;  _dlist1[0] = &(Dm1) - _p;
 _slist1[1] = &(h1) - _p;  _dlist1[1] = &(Dh1) - _p;
 _slist1[2] = &(m2) - _p;  _dlist1[2] = &(Dm2) - _p;
   _t_minf1 = makevector(501*sizeof(double));
   _t_minf2 = makevector(501*sizeof(double));
   _t_hinf1 = makevector(501*sizeof(double));
   _t_taum1 = makevector(501*sizeof(double));
   _t_taum2 = makevector(501*sizeof(double));
   _t_tauh1 = makevector(501*sizeof(double));
_first = 0;
}

#if defined(__cplusplus)
} /* extern "C" */
#endif

#if NMODL_TEXT
static const char* nmodl_filename = "kd2.mod";
static const char* nmodl_file_text = 
  ": Kd current fit from Ransdell & Schulz 2011 data\n"
  "\n"
  "\n"
  "NEURON {\n"
  "	SUFFIX kd2\n"
  "	USEION k READ ek WRITE ik\n"
  "	RANGE g1,g2,G1,G2\n"
  "	RANGE minf, taum, i\n"
  "\n"
  "	RANGE tbase, tamp, vhalf, k\n"
  "}\n"
  "\n"
  "UNITS {\n"
  "	(mA) = (milliamp)\n"
  "	(mV) = (millivolt)\n"
  "}\n"
  "\n"
  "PARAMETER {\n"
  "	g1 = 0.783e-6 (siemens/cm2) <0,1e9>\n"
  "	g2 = 0.3356e-6 (siemens/cm2) <0,1e9>\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "	v (mV)\n"
  "	ek (mV)\n"
  "	ik (mA/cm2)\n"
  "	G (siemens/cm2)\n"
  "	i\n"
  "}\n"
  "\n"
  "STATE {\n"
  "	m1 h1 m2\n"
  "}\n"
  "\n"
  "BREAKPOINT {\n"
  "	SOLVE states METHOD cnexp\n"
  "	G = g1*m1^4*h1 + g2*m2^4\n"
  "	i = G*(v-ek)\n"
  "	ik=i\n"
  "}\n"
  "\n"
  "INITIAL {\n"
  "	m1 = minf1(v)\n"
  "	h1 = hinf1(v)\n"
  "	m2 = minf2(v)\n"
  "}\n"
  "\n"
  "DERIVATIVE states {\n"
  "	m1' = (minf1(v)-m1)/taum1(v)\n"
  "	h1' = (hinf1(v)-h1)/tauh1(v)\n"
  "	m2' = (minf2(v)-m2)/taum2(v)\n"
  "}\n"
  "\n"
  "\n"
  "FUNCTION minf1(v(mV)) {\n"
  "	TABLE FROM -150 TO 150 WITH 500\n"
  "	minf1 = 1/(1+exp((v+24.19)/-10.77))\n"
  "}\n"
  "\n"
  "FUNCTION minf2(v(mV)) {\n"
  "	TABLE FROM -150 TO 150 WITH 500\n"
  "	minf2 = 1/(1+exp((v+23.32)/-10))   : Fit from Schulz data\n"
  "}\n"
  "\n"
  "FUNCTION hinf1(v(mV)) {\n"
  "	TABLE FROM -150 TO 150 WITH 500\n"
  "	hinf1 = 0.3 + ((1-0.3)/(1+exp((v+15.87)/5.916)))  :Fit from Schulz data\n"
  "}\n"
  "\n"
  "\n"
  "\n"
  "FUNCTION taum1(v(mV)) {\n"
  "	TABLE FROM -150 TO 150 WITH 500\n"
  "	taum1 = 25.049 + 25/(1+exp((v+25.84)/6.252))\n"
  "	:taum1 = 5.049 + 25/(1+exp((v+25.84)/6.252))\n"
  "}\n"
  "\n"
  "FUNCTION taum2(v(mV)) {\n"
  "	TABLE FROM -150 TO 150 WITH 500\n"
  "	taum2 = 100 + 550/(1+exp((v+15)/12.46))\n"
  "}\n"
  "\n"
  "FUNCTION tauh1(v(mV)) {\n"
  "	TABLE FROM -150 TO 150 WITH 500\n"
  "    tauh1 = 550 + 954.9/(1+exp((v+10.8)/-15))  :tauh1 = 600 + 7936/(1+exp((v+13.98)/3.095))   \n"
  "}\n"
  ;
#endif
