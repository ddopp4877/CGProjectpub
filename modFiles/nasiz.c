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
 
#define nrn_init _nrn_init__nasiz
#define _nrn_initial _nrn_initial__nasiz
#define nrn_cur _nrn_cur__nasiz
#define _nrn_current _nrn_current__nasiz
#define nrn_jacob _nrn_jacob__nasiz
#define nrn_state _nrn_state__nasiz
#define _net_receive _net_receive__nasiz 
#define rate rate__nasiz 
#define states states__nasiz 
 
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
#define g _p[0]
#define G _p[1]
#define minf _p[2]
#define taum _p[3]
#define hinf _p[4]
#define tauh _p[5]
#define i _p[6]
#define m _p[7]
#define h _p[8]
#define ek _p[9]
#define ik _p[10]
#define ina _p[11]
#define ena _p[12]
#define Dm _p[13]
#define Dh _p[14]
#define v _p[15]
#define _g _p[16]
#define _ion_ena	*_ppvar[0]._pval
#define _ion_ina	*_ppvar[1]._pval
#define _ion_dinadv	*_ppvar[2]._pval
 
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
 static void _hoc_hinffun(void);
 static void _hoc_minffun(void);
 static void _hoc_rate(void);
 static void _hoc_tauhfun(void);
 static void _hoc_taumfun(void);
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
 "setdata_nasiz", _hoc_setdata,
 "hinffun_nasiz", _hoc_hinffun,
 "minffun_nasiz", _hoc_minffun,
 "rate_nasiz", _hoc_rate,
 "tauhfun_nasiz", _hoc_tauhfun,
 "taumfun_nasiz", _hoc_taumfun,
 0, 0
};
#define _f_tauhfun _f_tauhfun_nasiz
#define _f_hinffun _f_hinffun_nasiz
#define _f_taumfun _f_taumfun_nasiz
#define _f_minffun _f_minffun_nasiz
#define hinffun hinffun_nasiz
#define minffun minffun_nasiz
#define tauhfun tauhfun_nasiz
#define taumfun taumfun_nasiz
 extern double _f_tauhfun( _threadargsprotocomma_ double );
 extern double _f_hinffun( _threadargsprotocomma_ double );
 extern double _f_taumfun( _threadargsprotocomma_ double );
 extern double _f_minffun( _threadargsprotocomma_ double );
 extern double hinffun( _threadargsprotocomma_ double );
 extern double minffun( _threadargsprotocomma_ double );
 extern double tauhfun( _threadargsprotocomma_ double );
 extern double taumfun( _threadargsprotocomma_ double );
 
static void _check_minffun(double*, Datum*, Datum*, _NrnThread*); 
static void _check_taumfun(double*, Datum*, Datum*, _NrnThread*); 
static void _check_hinffun(double*, Datum*, Datum*, _NrnThread*); 
static void _check_tauhfun(double*, Datum*, Datum*, _NrnThread*); 
static void _check_table_thread(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, int _type) {
   _check_minffun(_p, _ppvar, _thread, _nt);
   _check_taumfun(_p, _ppvar, _thread, _nt);
   _check_hinffun(_p, _ppvar, _thread, _nt);
   _check_tauhfun(_p, _ppvar, _thread, _nt);
 }
 /* declare global and static user variables */
#define usetable usetable_nasiz
 double usetable = 1;
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 "g_nasiz", 0, 1e+09,
 "usetable_nasiz", 0, 1,
 0,0,0
};
 static HocParmUnits _hoc_parm_units[] = {
 "g_nasiz", "siemens/cm2",
 "G_nasiz", "siemens/cm2",
 "taum_nasiz", "ms",
 "tauh_nasiz", "ms",
 0,0
};
 static double delta_t = 0.01;
 static double h0 = 0;
 static double m0 = 0;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 "usetable_nasiz", &usetable_nasiz,
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
"nasiz",
 "g_nasiz",
 0,
 "G_nasiz",
 "minf_nasiz",
 "taum_nasiz",
 "hinf_nasiz",
 "tauh_nasiz",
 "i_nasiz",
 0,
 "m_nasiz",
 "h_nasiz",
 0,
 0};
 static Symbol* _na_sym;
 
extern Prop* need_memb(Symbol*);

static void nrn_alloc(Prop* _prop) {
	Prop *prop_ion;
	double *_p; Datum *_ppvar;
 	_p = nrn_prop_data_alloc(_mechtype, 17, _prop);
 	/*initialize range parameters*/
 	g = 0.012;
 	_prop->param = _p;
 	_prop->param_size = 17;
 	_ppvar = nrn_prop_datum_alloc(_mechtype, 4, _prop);
 	_prop->dparam = _ppvar;
 	/*connect ionic variables to this model*/
 prop_ion = need_memb(_na_sym);
 nrn_promote(prop_ion, 0, 1);
 	_ppvar[0]._pval = &prop_ion->param[0]; /* ena */
 	_ppvar[1]._pval = &prop_ion->param[3]; /* ina */
 	_ppvar[2]._pval = &prop_ion->param[4]; /* _ion_dinadv */
 
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

 void _nasiz_reg() {
	int _vectorized = 1;
  _initlists();
 	ion_reg("na", -10000.);
 	_na_sym = hoc_lookup("na_ion");
 	register_mech(_mechanism, nrn_alloc,nrn_cur, nrn_jacob, nrn_state, nrn_init, hoc_nrnpointerindex, 1);
 _mechtype = nrn_get_mechtype(_mechanism[1]);
     _nrn_setdata_reg(_mechtype, _setdata);
     _nrn_thread_reg(_mechtype, 2, _update_ion_pointer);
     _nrn_thread_table_reg(_mechtype, _check_table_thread);
 #if NMODL_TEXT
  hoc_reg_nmodl_text(_mechtype, nmodl_file_text);
  hoc_reg_nmodl_filename(_mechtype, nmodl_filename);
#endif
  hoc_register_prop_size(_mechtype, 17, 4);
  hoc_register_dparam_semantics(_mechtype, 0, "na_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "na_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "na_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "cvodeieq");
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 	hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 nasiz C:/Users/ddopp/source/repos/CGProjectpub/modFiles/nasiz.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
 static double *_t_minffun;
 static double *_t_taumfun;
 static double *_t_hinffun;
 static double *_t_tauhfun;
static int _reset;
static char *modelname = "";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
static int rate(_threadargsprotocomma_ double);
 
static int _ode_spec1(_threadargsproto_);
/*static int _ode_matsol1(_threadargsproto_);*/
 static double _n_tauhfun(_threadargsprotocomma_ double _lv);
 static double _n_hinffun(_threadargsprotocomma_ double _lv);
 static double _n_taumfun(_threadargsprotocomma_ double _lv);
 static double _n_minffun(_threadargsprotocomma_ double _lv);
 static int _slist1[2], _dlist1[2];
 static int states(_threadargsproto_);
 
/*CVODE*/
 static int _ode_spec1 (double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {int _reset = 0; {
   rate ( _threadargscomma_ v ) ;
   Dm = ( minf - m ) / taum ;
   Dh = ( hinf - h ) / tauh ;
   }
 return _reset;
}
 static int _ode_matsol1 (double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {
 rate ( _threadargscomma_ v ) ;
 Dm = Dm  / (1. - dt*( ( ( ( - 1.0 ) ) ) / taum )) ;
 Dh = Dh  / (1. - dt*( ( ( ( - 1.0 ) ) ) / tauh )) ;
  return 0;
}
 /*END CVODE*/
 static int states (double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) { {
   rate ( _threadargscomma_ v ) ;
    m = m + (1. - exp(dt*(( ( ( - 1.0 ) ) ) / taum)))*(- ( ( ( minf ) ) / taum ) / ( ( ( ( - 1.0 ) ) ) / taum ) - m) ;
    h = h + (1. - exp(dt*(( ( ( - 1.0 ) ) ) / tauh)))*(- ( ( ( hinf ) ) / tauh ) / ( ( ( ( - 1.0 ) ) ) / tauh ) - h) ;
   }
  return 0;
}
 
static int  rate ( _threadargsprotocomma_ double _lv ) {
    minf = minffun ( _threadargscomma_ _lv ) ;
   taum = taumfun ( _threadargscomma_ _lv ) ;
   hinf = hinffun ( _threadargscomma_ _lv ) ;
   tauh = tauhfun ( _threadargscomma_ _lv ) ;
     return 0; }
 
static void _hoc_rate(void) {
  double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   if (_extcall_prop) {_p = _extcall_prop->param; _ppvar = _extcall_prop->dparam;}else{ _p = (double*)0; _ppvar = (Datum*)0; }
  _thread = _extcall_thread;
  _nt = nrn_threads;
 _r = 1.;
 rate ( _p, _ppvar, _thread, _nt, *getarg(1) );
 hoc_retpushx(_r);
}
 static double _mfac_minffun, _tmin_minffun;
  static void _check_minffun(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {
  static int _maktable=1; int _i, _j, _ix = 0;
  double _xi, _tmax;
  if (!usetable) {return;}
  if (_maktable) { double _x, _dx; _maktable=0;
   _tmin_minffun =  - 150.0 ;
   _tmax =  150.0 ;
   _dx = (_tmax - _tmin_minffun)/500.; _mfac_minffun = 1./_dx;
   for (_i=0, _x=_tmin_minffun; _i < 501; _x += _dx, _i++) {
    _t_minffun[_i] = _f_minffun(_p, _ppvar, _thread, _nt, _x);
   }
  }
 }

 double minffun(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _lv) { 
#if 0
_check_minffun(_p, _ppvar, _thread, _nt);
#endif
 return _n_minffun(_p, _ppvar, _thread, _nt, _lv);
 }

 static double _n_minffun(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _lv){ int _i, _j;
 double _xi, _theta;
 if (!usetable) {
 return _f_minffun(_p, _ppvar, _thread, _nt, _lv); 
}
 _xi = _mfac_minffun * (_lv - _tmin_minffun);
 if (isnan(_xi)) {
  return _xi; }
 if (_xi <= 0.) {
 return _t_minffun[0];
 }
 if (_xi >= 500.) {
 return _t_minffun[500];
 }
 _i = (int) _xi;
 return _t_minffun[_i] + (_xi - (double)_i)*(_t_minffun[_i+1] - _t_minffun[_i]);
 }

 
double _f_minffun ( _threadargsprotocomma_ double _lv ) {
   double _lminffun;
 _lminffun = 1.0 / ( 1.0 + exp ( ( _lv + 20.5 ) / - 5.29 ) ) ;
   
return _lminffun;
 }
 
static void _hoc_minffun(void) {
  double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   if (_extcall_prop) {_p = _extcall_prop->param; _ppvar = _extcall_prop->dparam;}else{ _p = (double*)0; _ppvar = (Datum*)0; }
  _thread = _extcall_thread;
  _nt = nrn_threads;
 
#if 1
 _check_minffun(_p, _ppvar, _thread, _nt);
#endif
 _r =  minffun ( _p, _ppvar, _thread, _nt, *getarg(1) );
 hoc_retpushx(_r);
}
 static double _mfac_taumfun, _tmin_taumfun;
  static void _check_taumfun(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {
  static int _maktable=1; int _i, _j, _ix = 0;
  double _xi, _tmax;
  if (!usetable) {return;}
  if (_maktable) { double _x, _dx; _maktable=0;
   _tmin_taumfun =  - 150.0 ;
   _tmax =  150.0 ;
   _dx = (_tmax - _tmin_taumfun)/500.; _mfac_taumfun = 1./_dx;
   for (_i=0, _x=_tmin_taumfun; _i < 501; _x += _dx, _i++) {
    _t_taumfun[_i] = _f_taumfun(_p, _ppvar, _thread, _nt, _x);
   }
  }
 }

 double taumfun(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _lv) { 
#if 0
_check_taumfun(_p, _ppvar, _thread, _nt);
#endif
 return _n_taumfun(_p, _ppvar, _thread, _nt, _lv);
 }

 static double _n_taumfun(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _lv){ int _i, _j;
 double _xi, _theta;
 if (!usetable) {
 return _f_taumfun(_p, _ppvar, _thread, _nt, _lv); 
}
 _xi = _mfac_taumfun * (_lv - _tmin_taumfun);
 if (isnan(_xi)) {
  return _xi; }
 if (_xi <= 0.) {
 return _t_taumfun[0];
 }
 if (_xi >= 500.) {
 return _t_taumfun[500];
 }
 _i = (int) _xi;
 return _t_taumfun[_i] + (_xi - (double)_i)*(_t_taumfun[_i+1] - _t_taumfun[_i]);
 }

 
double _f_taumfun ( _threadargsprotocomma_ double _lv ) {
   double _ltaumfun;
 _ltaumfun = 1.34 - 1.26 / ( 1.0 + exp ( ( _lv + 120.0 ) / - 25.0 ) ) ;
   
return _ltaumfun;
 }
 
static void _hoc_taumfun(void) {
  double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   if (_extcall_prop) {_p = _extcall_prop->param; _ppvar = _extcall_prop->dparam;}else{ _p = (double*)0; _ppvar = (Datum*)0; }
  _thread = _extcall_thread;
  _nt = nrn_threads;
 
#if 1
 _check_taumfun(_p, _ppvar, _thread, _nt);
#endif
 _r =  taumfun ( _p, _ppvar, _thread, _nt, *getarg(1) );
 hoc_retpushx(_r);
}
 static double _mfac_hinffun, _tmin_hinffun;
  static void _check_hinffun(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {
  static int _maktable=1; int _i, _j, _ix = 0;
  double _xi, _tmax;
  if (!usetable) {return;}
  if (_maktable) { double _x, _dx; _maktable=0;
   _tmin_hinffun =  - 150.0 ;
   _tmax =  150.0 ;
   _dx = (_tmax - _tmin_hinffun)/500.; _mfac_hinffun = 1./_dx;
   for (_i=0, _x=_tmin_hinffun; _i < 501; _x += _dx, _i++) {
    _t_hinffun[_i] = _f_hinffun(_p, _ppvar, _thread, _nt, _x);
   }
  }
 }

 double hinffun(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _lv) { 
#if 0
_check_hinffun(_p, _ppvar, _thread, _nt);
#endif
 return _n_hinffun(_p, _ppvar, _thread, _nt, _lv);
 }

 static double _n_hinffun(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _lv){ int _i, _j;
 double _xi, _theta;
 if (!usetable) {
 return _f_hinffun(_p, _ppvar, _thread, _nt, _lv); 
}
 _xi = _mfac_hinffun * (_lv - _tmin_hinffun);
 if (isnan(_xi)) {
  return _xi; }
 if (_xi <= 0.) {
 return _t_hinffun[0];
 }
 if (_xi >= 500.) {
 return _t_hinffun[500];
 }
 _i = (int) _xi;
 return _t_hinffun[_i] + (_xi - (double)_i)*(_t_hinffun[_i+1] - _t_hinffun[_i]);
 }

 
double _f_hinffun ( _threadargsprotocomma_ double _lv ) {
   double _lhinffun;
 _lhinffun = 1.0 / ( 1.0 + exp ( ( _lv + 35.9 ) / 5.18 ) ) ;
   
return _lhinffun;
 }
 
static void _hoc_hinffun(void) {
  double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   if (_extcall_prop) {_p = _extcall_prop->param; _ppvar = _extcall_prop->dparam;}else{ _p = (double*)0; _ppvar = (Datum*)0; }
  _thread = _extcall_thread;
  _nt = nrn_threads;
 
#if 1
 _check_hinffun(_p, _ppvar, _thread, _nt);
#endif
 _r =  hinffun ( _p, _ppvar, _thread, _nt, *getarg(1) );
 hoc_retpushx(_r);
}
 static double _mfac_tauhfun, _tmin_tauhfun;
  static void _check_tauhfun(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {
  static int _maktable=1; int _i, _j, _ix = 0;
  double _xi, _tmax;
  if (!usetable) {return;}
  if (_maktable) { double _x, _dx; _maktable=0;
   _tmin_tauhfun =  - 150.0 ;
   _tmax =  150.0 ;
   _dx = (_tmax - _tmin_tauhfun)/500.; _mfac_tauhfun = 1./_dx;
   for (_i=0, _x=_tmin_tauhfun; _i < 501; _x += _dx, _i++) {
    _t_tauhfun[_i] = _f_tauhfun(_p, _ppvar, _thread, _nt, _x);
   }
  }
 }

 double tauhfun(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _lv) { 
#if 0
_check_tauhfun(_p, _ppvar, _thread, _nt);
#endif
 return _n_tauhfun(_p, _ppvar, _thread, _nt, _lv);
 }

 static double _n_tauhfun(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _lv){ int _i, _j;
 double _xi, _theta;
 if (!usetable) {
 return _f_tauhfun(_p, _ppvar, _thread, _nt, _lv); 
}
 _xi = _mfac_tauhfun * (_lv - _tmin_tauhfun);
 if (isnan(_xi)) {
  return _xi; }
 if (_xi <= 0.) {
 return _t_tauhfun[0];
 }
 if (_xi >= 500.) {
 return _t_tauhfun[500];
 }
 _i = (int) _xi;
 return _t_tauhfun[_i] + (_xi - (double)_i)*(_t_tauhfun[_i+1] - _t_tauhfun[_i]);
 }

 
double _f_tauhfun ( _threadargsprotocomma_ double _lv ) {
   double _ltauhfun;
 _ltauhfun = ( 0.67 / ( 1.0 + exp ( ( _lv + 62.9 ) / - 10.0 ) ) ) * ( 1.5 + 1.0 / ( 1.0 + exp ( ( _lv + 34.9 ) / 3.6 ) ) ) ;
   
return _ltauhfun;
 }
 
static void _hoc_tauhfun(void) {
  double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   if (_extcall_prop) {_p = _extcall_prop->param; _ppvar = _extcall_prop->dparam;}else{ _p = (double*)0; _ppvar = (Datum*)0; }
  _thread = _extcall_thread;
  _nt = nrn_threads;
 
#if 1
 _check_tauhfun(_p, _ppvar, _thread, _nt);
#endif
 _r =  tauhfun ( _p, _ppvar, _thread, _nt, *getarg(1) );
 hoc_retpushx(_r);
}
 
static int _ode_count(int _type){ return 2;}
 
static void _ode_spec(_NrnThread* _nt, _Memb_list* _ml, int _type) {
   double* _p; Datum* _ppvar; Datum* _thread;
   Node* _nd; double _v; int _iml, _cntml;
  _cntml = _ml->_nodecount;
  _thread = _ml->_thread;
  for (_iml = 0; _iml < _cntml; ++_iml) {
    _p = _ml->_data[_iml]; _ppvar = _ml->_pdata[_iml];
    _nd = _ml->_nodelist[_iml];
    v = NODEV(_nd);
  ena = _ion_ena;
     _ode_spec1 (_p, _ppvar, _thread, _nt);
  }}
 
static void _ode_map(int _ieq, double** _pv, double** _pvdot, double* _pp, Datum* _ppd, double* _atol, int _type) { 
	double* _p; Datum* _ppvar;
 	int _i; _p = _pp; _ppvar = _ppd;
	_cvode_ieq = _ieq;
	for (_i=0; _i < 2; ++_i) {
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
  ena = _ion_ena;
 _ode_matsol_instance1(_threadargs_);
 }}
 extern void nrn_update_ion_pointer(Symbol*, Datum*, int, int);
 static void _update_ion_pointer(Datum* _ppvar) {
   nrn_update_ion_pointer(_na_sym, _ppvar, 0, 0);
   nrn_update_ion_pointer(_na_sym, _ppvar, 1, 3);
   nrn_update_ion_pointer(_na_sym, _ppvar, 2, 4);
 }

static void initmodel(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {
  int _i; double _save;{
  h = h0;
  m = m0;
 {
   rate ( _threadargscomma_ v ) ;
   m = minf ;
   h = hinf ;
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
 _check_minffun(_p, _ppvar, _thread, _nt);
 _check_taumfun(_p, _ppvar, _thread, _nt);
 _check_hinffun(_p, _ppvar, _thread, _nt);
 _check_tauhfun(_p, _ppvar, _thread, _nt);
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
  ena = _ion_ena;
 initmodel(_p, _ppvar, _thread, _nt);
 }
}

static double _nrn_current(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _v){double _current=0.;v=_v;{ {
   G = g * m * m * m * h ;
   i = G * ( v - ena ) ;
   ina = i ;
   }
 _current += ina;

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
  ena = _ion_ena;
 _g = _nrn_current(_p, _ppvar, _thread, _nt, _v + .001);
 	{ double _dina;
  _dina = ina;
 _rhs = _nrn_current(_p, _ppvar, _thread, _nt, _v);
  _ion_dinadv += (_dina - ina)/.001 ;
 	}
 _g = (_g - _rhs)/.001;
  _ion_ina += ina ;
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
  ena = _ion_ena;
 {   states(_p, _ppvar, _thread, _nt);
  } }}

}

static void terminal(){}

static void _initlists(){
 double _x; double* _p = &_x;
 int _i; static int _first = 1;
  if (!_first) return;
 _slist1[0] = &(m) - _p;  _dlist1[0] = &(Dm) - _p;
 _slist1[1] = &(h) - _p;  _dlist1[1] = &(Dh) - _p;
   _t_minffun = makevector(501*sizeof(double));
   _t_taumfun = makevector(501*sizeof(double));
   _t_hinffun = makevector(501*sizeof(double));
   _t_tauhfun = makevector(501*sizeof(double));
_first = 0;
}

#if defined(__cplusplus)
} /* extern "C" */
#endif

#if NMODL_TEXT
static const char* nmodl_filename = "nasiz.mod";
static const char* nmodl_file_text = 
  ":Fast spiking sodium current from Turrigiano et al. 1995\n"
  "\n"
  "\n"
  "NEURON {\n"
  "	SUFFIX nasiz\n"
  "	USEION na READ ena WRITE ina\n"
  "	RANGE g, G\n"
  "	RANGE minf, taum, hinf, tauh, i\n"
  "	\n"
  "}\n"
  "\n"
  "UNITS {\n"
  "	(mA) = (milliamp)\n"
  "	(mV) = (millivolt)\n"
  "}\n"
  "\n"
  "PARAMETER {\n"
  "	g = 0.012 (siemens/cm2) <0,1e9>\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "	v (mV)\n"
  "	ek (mV)\n"
  "	ik (mA/cm2)\n"
  "	G (siemens/cm2)\n"
  "	minf\n"
  "	taum (ms)\n"
  "	hinf\n"
  "	tauh (ms)\n"
  "	i\n"
  "	ina\n"
  "	ena (mV)\n"
  "}\n"
  "\n"
  "STATE {\n"
  "	m\n"
  "	h\n"
  "}\n"
  "\n"
  "BREAKPOINT {\n"
  "	SOLVE states METHOD cnexp\n"
  "	G = g*m*m*m*h\n"
  "	i = G*(v-ena)\n"
  "	ina=i\n"
  "}\n"
  "\n"
  "INITIAL {\n"
  "	rate(v)\n"
  "	m = minf\n"
  "	h = hinf\n"
  "}\n"
  "\n"
  "DERIVATIVE states {\n"
  "	rate(v)\n"
  "	m' = (minf-m)/taum\n"
  "	h' = (hinf-h)/tauh\n"
  "}\n"
  "\n"
  "PROCEDURE rate(v (mV)) {\n"
  "	UNITSOFF\n"
  "	\n"
  "	minf = minffun(v)\n"
  "	taum = taumfun(v)\n"
  "	\n"
  "	hinf = hinffun(v)\n"
  "	tauh = tauhfun(v)\n"
  "	\n"
  "	UNITSON\n"
  "}\n"
  "\n"
  "FUNCTION minffun(v(mV)) {\n"
  "	TABLE FROM -150 TO 150 WITH 500\n"
  "	minffun = 1/(1+exp((v+20.5)/-5.29))\n"
  "}\n"
  "\n"
  "FUNCTION taumfun(v(mV)) {\n"
  "	TABLE FROM -150 TO 150 WITH 500\n"
  "	taumfun = 1.34 - 1.26/(1+exp((v+120)/-25))\n"
  "}\n"
  "\n"
  "FUNCTION hinffun(v(mV)) {\n"
  "	TABLE FROM -150 TO 150 WITH 500\n"
  "	hinffun = 1/(1+exp((v+35.9)/5.18))\n"
  "}\n"
  "\n"
  "FUNCTION tauhfun(v(mV)) {\n"
  "	TABLE FROM -150 TO 150 WITH 500\n"
  "	tauhfun = (0.67/(1+exp((v+62.9)/-10)))*(1.5 + 1/(1+exp((v+34.9)/3.6)))\n"
  "}\n"
  ;
#endif