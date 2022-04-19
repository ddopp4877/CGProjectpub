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
 
#define nrn_init _nrn_init__cat
#define _nrn_initial _nrn_initial__cat
#define nrn_cur _nrn_cur__cat
#define _nrn_current _nrn_current__cat
#define nrn_jacob _nrn_jacob__cat
#define nrn_state _nrn_state__cat
#define _net_receive _net_receive__cat 
#define states states__cat 
 
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
#define i _p[1]
#define G _p[2]
#define m _p[3]
#define h _p[4]
#define ek _p[5]
#define eca _p[6]
#define ica _p[7]
#define Dm _p[8]
#define Dh _p[9]
#define v _p[10]
#define _g _p[11]
#define _ion_eca	*_ppvar[0]._pval
#define _ion_ica	*_ppvar[1]._pval
#define _ion_dicadv	*_ppvar[2]._pval
 
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
 static void _hoc_hinf(void);
 static void _hoc_minf(void);
 static void _hoc_tauh(void);
 static void _hoc_taum(void);
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
 "setdata_cat", _hoc_setdata,
 "hinf_cat", _hoc_hinf,
 "minf_cat", _hoc_minf,
 "tauh_cat", _hoc_tauh,
 "taum_cat", _hoc_taum,
 0, 0
};
#define _f_tauh _f_tauh_cat
#define _f_hinf _f_hinf_cat
#define _f_taum _f_taum_cat
#define _f_minf _f_minf_cat
#define hinf hinf_cat
#define minf minf_cat
#define tauh tauh_cat
#define taum taum_cat
 extern double _f_tauh( _threadargsprotocomma_ double );
 extern double _f_hinf( _threadargsprotocomma_ double );
 extern double _f_taum( _threadargsprotocomma_ double );
 extern double _f_minf( _threadargsprotocomma_ double );
 extern double hinf( _threadargsprotocomma_ double );
 extern double minf( _threadargsprotocomma_ double );
 extern double tauh( _threadargsprotocomma_ double );
 extern double taum( _threadargsprotocomma_ double );
 
static void _check_minf(double*, Datum*, Datum*, _NrnThread*); 
static void _check_taum(double*, Datum*, Datum*, _NrnThread*); 
static void _check_hinf(double*, Datum*, Datum*, _NrnThread*); 
static void _check_tauh(double*, Datum*, Datum*, _NrnThread*); 
static void _check_table_thread(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, int _type) {
   _check_minf(_p, _ppvar, _thread, _nt);
   _check_taum(_p, _ppvar, _thread, _nt);
   _check_hinf(_p, _ppvar, _thread, _nt);
   _check_tauh(_p, _ppvar, _thread, _nt);
 }
 /* declare global and static user variables */
#define usetable usetable_cat
 double usetable = 1;
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 "g_cat", 0, 1e+09,
 "usetable_cat", 0, 1,
 0,0,0
};
 static HocParmUnits _hoc_parm_units[] = {
 "g_cat", "siemens/cm2",
 "G_cat", "siemens/cm2",
 0,0
};
 static double delta_t = 0.01;
 static double h0 = 0;
 static double m0 = 0;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 "usetable_cat", &usetable_cat,
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
"cat",
 "g_cat",
 0,
 "i_cat",
 "G_cat",
 0,
 "m_cat",
 "h_cat",
 0,
 0};
 static Symbol* _ca_sym;
 
extern Prop* need_memb(Symbol*);

static void nrn_alloc(Prop* _prop) {
	Prop *prop_ion;
	double *_p; Datum *_ppvar;
 	_p = nrn_prop_data_alloc(_mechtype, 12, _prop);
 	/*initialize range parameters*/
 	g = 5.097e-05;
 	_prop->param = _p;
 	_prop->param_size = 12;
 	_ppvar = nrn_prop_datum_alloc(_mechtype, 4, _prop);
 	_prop->dparam = _ppvar;
 	/*connect ionic variables to this model*/
 prop_ion = need_memb(_ca_sym);
 nrn_promote(prop_ion, 0, 1);
 	_ppvar[0]._pval = &prop_ion->param[0]; /* eca */
 	_ppvar[1]._pval = &prop_ion->param[3]; /* ica */
 	_ppvar[2]._pval = &prop_ion->param[4]; /* _ion_dicadv */
 
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

 void _cat_reg() {
	int _vectorized = 1;
  _initlists();
 	ion_reg("ca", -10000.);
 	_ca_sym = hoc_lookup("ca_ion");
 	register_mech(_mechanism, nrn_alloc,nrn_cur, nrn_jacob, nrn_state, nrn_init, hoc_nrnpointerindex, 1);
 _mechtype = nrn_get_mechtype(_mechanism[1]);
     _nrn_setdata_reg(_mechtype, _setdata);
     _nrn_thread_reg(_mechtype, 2, _update_ion_pointer);
     _nrn_thread_table_reg(_mechtype, _check_table_thread);
 #if NMODL_TEXT
  hoc_reg_nmodl_text(_mechtype, nmodl_file_text);
  hoc_reg_nmodl_filename(_mechtype, nmodl_filename);
#endif
  hoc_register_prop_size(_mechtype, 12, 4);
  hoc_register_dparam_semantics(_mechtype, 0, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "cvodeieq");
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 	hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 cat C:/Users/ddopp/source/repos/CGProjectpub/modFiles/cat.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
 
#define FARADAY _nrnunit_FARADAY[_nrnunit_use_legacy_]
static double _nrnunit_FARADAY[2] = {0xb.c72aa8304416p+13, 96485.3}; /* 96485.3321233100141 */
 
#define R _nrnunit_R[_nrnunit_use_legacy_]
static double _nrnunit_R[2] = {0x8.50809f44c85fp+0, 8.3145}; /* 8.3144626181532395 */
 static double *_t_minf;
 static double *_t_taum;
 static double *_t_hinf;
 static double *_t_tauh;
static int _reset;
static char *modelname = "";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
 
static int _ode_spec1(_threadargsproto_);
/*static int _ode_matsol1(_threadargsproto_);*/
 static double _n_tauh(_threadargsprotocomma_ double _lv);
 static double _n_hinf(_threadargsprotocomma_ double _lv);
 static double _n_taum(_threadargsprotocomma_ double _lv);
 static double _n_minf(_threadargsprotocomma_ double _lv);
 static int _slist1[2], _dlist1[2];
 static int states(_threadargsproto_);
 
/*CVODE*/
 static int _ode_spec1 (double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {int _reset = 0; {
   Dm = ( minf ( _threadargscomma_ v ) - m ) / taum ( _threadargscomma_ v ) ;
   Dh = ( hinf ( _threadargscomma_ v ) - h ) / tauh ( _threadargscomma_ v ) ;
   }
 return _reset;
}
 static int _ode_matsol1 (double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {
 Dm = Dm  / (1. - dt*( ( ( ( - 1.0 ) ) ) / taum ( _threadargscomma_ v ) )) ;
 Dh = Dh  / (1. - dt*( ( ( ( - 1.0 ) ) ) / tauh ( _threadargscomma_ v ) )) ;
  return 0;
}
 /*END CVODE*/
 static int states (double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) { {
    m = m + (1. - exp(dt*(( ( ( - 1.0 ) ) ) / taum ( _threadargscomma_ v ))))*(- ( ( ( minf ( _threadargscomma_ v ) ) ) / taum ( _threadargscomma_ v ) ) / ( ( ( ( - 1.0 ) ) ) / taum ( _threadargscomma_ v ) ) - m) ;
    h = h + (1. - exp(dt*(( ( ( - 1.0 ) ) ) / tauh ( _threadargscomma_ v ))))*(- ( ( ( hinf ( _threadargscomma_ v ) ) ) / tauh ( _threadargscomma_ v ) ) / ( ( ( ( - 1.0 ) ) ) / tauh ( _threadargscomma_ v ) ) - h) ;
   }
  return 0;
}
 static double _mfac_minf, _tmin_minf;
  static void _check_minf(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {
  static int _maktable=1; int _i, _j, _ix = 0;
  double _xi, _tmax;
  if (!usetable) {return;}
  if (_maktable) { double _x, _dx; _maktable=0;
   _tmin_minf =  - 150.0 ;
   _tmax =  150.0 ;
   _dx = (_tmax - _tmin_minf)/500.; _mfac_minf = 1./_dx;
   for (_i=0, _x=_tmin_minf; _i < 501; _x += _dx, _i++) {
    _t_minf[_i] = _f_minf(_p, _ppvar, _thread, _nt, _x);
   }
  }
 }

 double minf(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _lv) { 
#if 0
_check_minf(_p, _ppvar, _thread, _nt);
#endif
 return _n_minf(_p, _ppvar, _thread, _nt, _lv);
 }

 static double _n_minf(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _lv){ int _i, _j;
 double _xi, _theta;
 if (!usetable) {
 return _f_minf(_p, _ppvar, _thread, _nt, _lv); 
}
 _xi = _mfac_minf * (_lv - _tmin_minf);
 if (isnan(_xi)) {
  return _xi; }
 if (_xi <= 0.) {
 return _t_minf[0];
 }
 if (_xi >= 500.) {
 return _t_minf[500];
 }
 _i = (int) _xi;
 return _t_minf[_i] + (_xi - (double)_i)*(_t_minf[_i+1] - _t_minf[_i]);
 }

 
double _f_minf ( _threadargsprotocomma_ double _lv ) {
   double _lminf;
 _lminf = 1.0 / ( 1.0 + exp ( ( _lv + 20.0 ) / - 1.898 ) ) ;
   
return _lminf;
 }
 
static void _hoc_minf(void) {
  double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   if (_extcall_prop) {_p = _extcall_prop->param; _ppvar = _extcall_prop->dparam;}else{ _p = (double*)0; _ppvar = (Datum*)0; }
  _thread = _extcall_thread;
  _nt = nrn_threads;
 
#if 1
 _check_minf(_p, _ppvar, _thread, _nt);
#endif
 _r =  minf ( _p, _ppvar, _thread, _nt, *getarg(1) );
 hoc_retpushx(_r);
}
 static double _mfac_taum, _tmin_taum;
  static void _check_taum(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {
  static int _maktable=1; int _i, _j, _ix = 0;
  double _xi, _tmax;
  if (!usetable) {return;}
  if (_maktable) { double _x, _dx; _maktable=0;
   _tmin_taum =  - 150.0 ;
   _tmax =  150.0 ;
   _dx = (_tmax - _tmin_taum)/500.; _mfac_taum = 1./_dx;
   for (_i=0, _x=_tmin_taum; _i < 501; _x += _dx, _i++) {
    _t_taum[_i] = _f_taum(_p, _ppvar, _thread, _nt, _x);
   }
  }
 }

 double taum(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _lv) { 
#if 0
_check_taum(_p, _ppvar, _thread, _nt);
#endif
 return _n_taum(_p, _ppvar, _thread, _nt, _lv);
 }

 static double _n_taum(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _lv){ int _i, _j;
 double _xi, _theta;
 if (!usetable) {
 return _f_taum(_p, _ppvar, _thread, _nt, _lv); 
}
 _xi = _mfac_taum * (_lv - _tmin_taum);
 if (isnan(_xi)) {
  return _xi; }
 if (_xi <= 0.) {
 return _t_taum[0];
 }
 if (_xi >= 500.) {
 return _t_taum[500];
 }
 _i = (int) _xi;
 return _t_taum[_i] + (_xi - (double)_i)*(_t_taum[_i+1] - _t_taum[_i]);
 }

 
double _f_taum ( _threadargsprotocomma_ double _lv ) {
   double _ltaum;
 _ltaum = 18.51 - 3.388 / ( exp ( ( _lv - 6.53 ) / 9.736 ) + exp ( ( _lv + 12.39 ) / - 2.525 ) ) ;
   
return _ltaum;
 }
 
static void _hoc_taum(void) {
  double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   if (_extcall_prop) {_p = _extcall_prop->param; _ppvar = _extcall_prop->dparam;}else{ _p = (double*)0; _ppvar = (Datum*)0; }
  _thread = _extcall_thread;
  _nt = nrn_threads;
 
#if 1
 _check_taum(_p, _ppvar, _thread, _nt);
#endif
 _r =  taum ( _p, _ppvar, _thread, _nt, *getarg(1) );
 hoc_retpushx(_r);
}
 static double _mfac_hinf, _tmin_hinf;
  static void _check_hinf(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {
  static int _maktable=1; int _i, _j, _ix = 0;
  double _xi, _tmax;
  if (!usetable) {return;}
  if (_maktable) { double _x, _dx; _maktable=0;
   _tmin_hinf =  - 150.0 ;
   _tmax =  150.0 ;
   _dx = (_tmax - _tmin_hinf)/500.; _mfac_hinf = 1./_dx;
   for (_i=0, _x=_tmin_hinf; _i < 501; _x += _dx, _i++) {
    _t_hinf[_i] = _f_hinf(_p, _ppvar, _thread, _nt, _x);
   }
  }
 }

 double hinf(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _lv) { 
#if 0
_check_hinf(_p, _ppvar, _thread, _nt);
#endif
 return _n_hinf(_p, _ppvar, _thread, _nt, _lv);
 }

 static double _n_hinf(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _lv){ int _i, _j;
 double _xi, _theta;
 if (!usetable) {
 return _f_hinf(_p, _ppvar, _thread, _nt, _lv); 
}
 _xi = _mfac_hinf * (_lv - _tmin_hinf);
 if (isnan(_xi)) {
  return _xi; }
 if (_xi <= 0.) {
 return _t_hinf[0];
 }
 if (_xi >= 500.) {
 return _t_hinf[500];
 }
 _i = (int) _xi;
 return _t_hinf[_i] + (_xi - (double)_i)*(_t_hinf[_i+1] - _t_hinf[_i]);
 }

 
double _f_hinf ( _threadargsprotocomma_ double _lv ) {
   double _lhinf;
 _lhinf = 1.0 / ( 1.0 + exp ( ( _lv + 55.27 ) / 6.11 ) ) ;
   
return _lhinf;
 }
 
static void _hoc_hinf(void) {
  double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   if (_extcall_prop) {_p = _extcall_prop->param; _ppvar = _extcall_prop->dparam;}else{ _p = (double*)0; _ppvar = (Datum*)0; }
  _thread = _extcall_thread;
  _nt = nrn_threads;
 
#if 1
 _check_hinf(_p, _ppvar, _thread, _nt);
#endif
 _r =  hinf ( _p, _ppvar, _thread, _nt, *getarg(1) );
 hoc_retpushx(_r);
}
 static double _mfac_tauh, _tmin_tauh;
  static void _check_tauh(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {
  static int _maktable=1; int _i, _j, _ix = 0;
  double _xi, _tmax;
  if (!usetable) {return;}
  if (_maktable) { double _x, _dx; _maktable=0;
   _tmin_tauh =  - 150.0 ;
   _tmax =  150.0 ;
   _dx = (_tmax - _tmin_tauh)/500.; _mfac_tauh = 1./_dx;
   for (_i=0, _x=_tmin_tauh; _i < 501; _x += _dx, _i++) {
    _t_tauh[_i] = _f_tauh(_p, _ppvar, _thread, _nt, _x);
   }
  }
 }

 double tauh(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _lv) { 
#if 0
_check_tauh(_p, _ppvar, _thread, _nt);
#endif
 return _n_tauh(_p, _ppvar, _thread, _nt, _lv);
 }

 static double _n_tauh(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _lv){ int _i, _j;
 double _xi, _theta;
 if (!usetable) {
 return _f_tauh(_p, _ppvar, _thread, _nt, _lv); 
}
 _xi = _mfac_tauh * (_lv - _tmin_tauh);
 if (isnan(_xi)) {
  return _xi; }
 if (_xi <= 0.) {
 return _t_tauh[0];
 }
 if (_xi >= 500.) {
 return _t_tauh[500];
 }
 _i = (int) _xi;
 return _t_tauh[_i] + (_xi - (double)_i)*(_t_tauh[_i+1] - _t_tauh[_i]);
 }

 
double _f_tauh ( _threadargsprotocomma_ double _lv ) {
   double _ltauh;
 _ltauh = 94.16 - 45.27 / ( 1.0 + exp ( ( _lv + 10.0 ) / - 10.0 ) ) ;
   
return _ltauh;
 }
 
static void _hoc_tauh(void) {
  double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   if (_extcall_prop) {_p = _extcall_prop->param; _ppvar = _extcall_prop->dparam;}else{ _p = (double*)0; _ppvar = (Datum*)0; }
  _thread = _extcall_thread;
  _nt = nrn_threads;
 
#if 1
 _check_tauh(_p, _ppvar, _thread, _nt);
#endif
 _r =  tauh ( _p, _ppvar, _thread, _nt, *getarg(1) );
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
  eca = _ion_eca;
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
  eca = _ion_eca;
 _ode_matsol_instance1(_threadargs_);
 }}
 extern void nrn_update_ion_pointer(Symbol*, Datum*, int, int);
 static void _update_ion_pointer(Datum* _ppvar) {
   nrn_update_ion_pointer(_ca_sym, _ppvar, 0, 0);
   nrn_update_ion_pointer(_ca_sym, _ppvar, 1, 3);
   nrn_update_ion_pointer(_ca_sym, _ppvar, 2, 4);
 }

static void initmodel(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {
  int _i; double _save;{
  h = h0;
  m = m0;
 {
   m = minf ( _threadargscomma_ v ) ;
   h = hinf ( _threadargscomma_ v ) ;
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
 _check_minf(_p, _ppvar, _thread, _nt);
 _check_taum(_p, _ppvar, _thread, _nt);
 _check_hinf(_p, _ppvar, _thread, _nt);
 _check_tauh(_p, _ppvar, _thread, _nt);
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
  eca = _ion_eca;
 initmodel(_p, _ppvar, _thread, _nt);
 }
}

static double _nrn_current(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _v){double _current=0.;v=_v;{ {
   G = g * m * h ;
   i = G * ( v - eca ) ;
   ica = i ;
   }
 _current += ica;

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
  eca = _ion_eca;
 _g = _nrn_current(_p, _ppvar, _thread, _nt, _v + .001);
 	{ double _dica;
  _dica = ica;
 _rhs = _nrn_current(_p, _ppvar, _thread, _nt, _v);
  _ion_dicadv += (_dica - ica)/.001 ;
 	}
 _g = (_g - _rhs)/.001;
  _ion_ica += ica ;
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
  eca = _ion_eca;
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
   _t_minf = makevector(501*sizeof(double));
   _t_taum = makevector(501*sizeof(double));
   _t_hinf = makevector(501*sizeof(double));
   _t_tauh = makevector(501*sizeof(double));
_first = 0;
}

#if defined(__cplusplus)
} /* extern "C" */
#endif

#if NMODL_TEXT
static const char* nmodl_filename = "cat.mod";
static const char* nmodl_file_text = 
  ": T-type calcium current fit from Joey's data 11-6-2011\n"
  "\n"
  "\n"
  "NEURON {\n"
  "	SUFFIX cat\n"
  "	USEION ca READ eca WRITE ica\n"
  "	RANGE G, g, i\n"
  "}\n"
  "\n"
  "UNITS {\n"
  "	(mA) = (milliamp)\n"
  "	(mV) = (millivolt)\n"
  "	FARADAY = (faraday) (coulomb)\n"
  "	R = (k-mole) (joule/degC)\n"
  "}\n"
  "\n"
  "PARAMETER {\n"
  "	g = 50.97e-6 (siemens/cm2) <0,1e9>\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "	v (mV)\n"
  "	ek (mV)\n"
  "	eca\n"
  "	i\n"
  "	ica (mA/cm2)\n"
  "	G (siemens/cm2)\n"
  "}\n"
  "\n"
  "STATE {\n"
  "	m\n"
  "	h\n"
  "}\n"
  "\n"
  "BREAKPOINT {\n"
  "	SOLVE states METHOD cnexp\n"
  "	G = g*m*h\n"
  "	i = G*(v-eca)\n"
  "	ica = i\n"
  "}\n"
  "\n"
  "INITIAL {\n"
  "	m = minf(v)\n"
  "	h = hinf(v)\n"
  "}\n"
  "\n"
  "DERIVATIVE states {\n"
  "	m' = (minf(v)-m)/taum(v)\n"
  "	h' = (hinf(v)-h)/tauh(v)\n"
  "}\n"
  "\n"
  "\n"
  "FUNCTION minf(v(mV)) {\n"
  "	TABLE FROM -150 TO 150 WITH 500\n"
  "	:minf =  0.8/(1+(exp((v+20.53)/-8.059))) : used before 17th sep 1/((exp((v+10.07)/9.919))+(exp((v+20.53)/-8.059)))\n"
  "	minf = 1/(1+exp((v+20)/-1.898))\n"
  "	:minf = 1/(1+exp((v+21.96)/-1.952))\n"
  "	:minf = 1/(1+exp((v+24)/-7.9))\n"
  "}\n"
  "\n"
  "FUNCTION taum(v(mV)) {\n"
  "	TABLE FROM -150 TO 150 WITH 500\n"
  "	:taum =  20 \n"
  "	:taum = 3.528 + 10.08/(exp((v+38.26)/-20.65)+exp((v+6.219)/15.14))\n"
  "	taum = 18.51 - 3.388/(exp((v-6.53)/9.736)+exp((v+12.39)/-2.525))\n"
  "}\n"
  "\n"
  "FUNCTION hinf(v(mV)) {\n"
  "	TABLE FROM -150 TO 150 WITH 500\n"
  "	:hinf = 1/(1+exp((v+39)/10))   \n"
  "	hinf = 1/(1+exp((v+55.27)/6.11))    \n"
  "}\n"
  "\n"
  "FUNCTION tauh(v(mV)) {\n"
  "	TABLE FROM -150 TO 150 WITH 500\n"
  "	:tauh = 20.23 + 40/(exp((v+23.48)/-9.976)+exp((v+5.196)/10.84))\n"
  "	tauh = 94.16 - 45.27/(1 + exp((v+10)/-10))\n"
  "	:tauh = 5 + 1/(exp((v+23.48)/-9.976)+exp((v+5.196)/10.84))\n"
  "	:tauh =  1\n"
  "\n"
  "}\n"
  ;
#endif
