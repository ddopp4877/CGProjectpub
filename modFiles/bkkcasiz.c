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
 
#define nrn_init _nrn_init__bkkcasiz
#define _nrn_initial _nrn_initial__bkkcasiz
#define nrn_cur _nrn_cur__bkkcasiz
#define _nrn_current _nrn_current__bkkcasiz
#define nrn_jacob _nrn_jacob__bkkcasiz
#define nrn_state _nrn_state__bkkcasiz
#define _net_receive _net_receive__bkkcasiz 
#define states states__bkkcasiz 
 
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
#define a _p[3]
#define b _p[4]
#define caci _p[5]
#define ek _p[6]
#define ik _p[7]
#define Da _p[8]
#define Db _p[9]
#define v _p[10]
#define _g _p[11]
#define _ion_ek	*_ppvar[0]._pval
#define _ion_ik	*_ppvar[1]._pval
#define _ion_dikdv	*_ppvar[2]._pval
#define _ion_caci	*_ppvar[3]._pval
 
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
 static void _hoc_ainf(void);
 static void _hoc_binf(void);
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
 "setdata_bkkcasiz", _hoc_setdata,
 "ainf_bkkcasiz", _hoc_ainf,
 "binf_bkkcasiz", _hoc_binf,
 0, 0
};
#define ainf ainf_bkkcasiz
#define binf binf_bkkcasiz
 extern double ainf( _threadargsprotocomma_ double , double );
 extern double binf( _threadargsprotocomma_ double );
 /* declare global and static user variables */
#define Vao2 Vao2_bkkcasiz
 double Vao2 = -5;
#define Vao1 Vao1_bkkcasiz
 double Vao1 = -5;
#define c3 c3_bkkcasiz
 double c3 = 15;
#define c2 c2_bkkcasiz
 double c2 = 14;
#define c1 c1_bkkcasiz
 double c1 = 0.0001;
#define f f_bkkcasiz
 double f = 0.08;
#define kCa kCa_bkkcasiz
 double kCa = 0.36;
#define kob kob_bkkcasiz
 double kob = 0.2;
#define koa koa_bkkcasiz
 double koa = 0.4;
#define sao2 sao2_bkkcasiz
 double sao2 = -9;
#define sao1 sao1_bkkcasiz
 double sao1 = -15;
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 "g_bkkcasiz", 0, 1e+09,
 0,0,0
};
 static HocParmUnits _hoc_parm_units[] = {
 "koa_bkkcasiz", "1/msec",
 "kob_bkkcasiz", "1/msec",
 "kCa_bkkcasiz", "1/msec",
 "Vao1_bkkcasiz", "mV",
 "Vao2_bkkcasiz", "mV",
 "g_bkkcasiz", "siemens/cm2",
 "G_bkkcasiz", "siemens/cm2",
 0,0
};
 static double a0 = 0;
 static double b0 = 0;
 static double delta_t = 0.01;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 "f_bkkcasiz", &f_bkkcasiz,
 "koa_bkkcasiz", &koa_bkkcasiz,
 "kob_bkkcasiz", &kob_bkkcasiz,
 "kCa_bkkcasiz", &kCa_bkkcasiz,
 "Vao1_bkkcasiz", &Vao1_bkkcasiz,
 "Vao2_bkkcasiz", &Vao2_bkkcasiz,
 "sao1_bkkcasiz", &sao1_bkkcasiz,
 "sao2_bkkcasiz", &sao2_bkkcasiz,
 "c1_bkkcasiz", &c1_bkkcasiz,
 "c2_bkkcasiz", &c2_bkkcasiz,
 "c3_bkkcasiz", &c3_bkkcasiz,
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
 
#define _cvode_ieq _ppvar[4]._i
 static void _ode_matsol_instance1(_threadargsproto_);
 /* connect range variables in _p that hoc is supposed to know about */
 static const char *_mechanism[] = {
 "7.7.0",
"bkkcasiz",
 "g_bkkcasiz",
 0,
 "i_bkkcasiz",
 "G_bkkcasiz",
 0,
 "a_bkkcasiz",
 "b_bkkcasiz",
 0,
 0};
 static Symbol* _k_sym;
 static Symbol* _cac_sym;
 
extern Prop* need_memb(Symbol*);

static void nrn_alloc(Prop* _prop) {
	Prop *prop_ion;
	double *_p; Datum *_ppvar;
 	_p = nrn_prop_data_alloc(_mechtype, 12, _prop);
 	/*initialize range parameters*/
 	g = 0.012;
 	_prop->param = _p;
 	_prop->param_size = 12;
 	_ppvar = nrn_prop_datum_alloc(_mechtype, 5, _prop);
 	_prop->dparam = _ppvar;
 	/*connect ionic variables to this model*/
 prop_ion = need_memb(_k_sym);
 nrn_promote(prop_ion, 0, 1);
 	_ppvar[0]._pval = &prop_ion->param[0]; /* ek */
 	_ppvar[1]._pval = &prop_ion->param[3]; /* ik */
 	_ppvar[2]._pval = &prop_ion->param[4]; /* _ion_dikdv */
 prop_ion = need_memb(_cac_sym);
 nrn_promote(prop_ion, 1, 0);
 	_ppvar[3]._pval = &prop_ion->param[1]; /* caci */
 
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

 void _bkkcasiz_reg() {
	int _vectorized = 1;
  _initlists();
 	ion_reg("k", -10000.);
 	ion_reg("cac", 2.0);
 	_k_sym = hoc_lookup("k_ion");
 	_cac_sym = hoc_lookup("cac_ion");
 	register_mech(_mechanism, nrn_alloc,nrn_cur, nrn_jacob, nrn_state, nrn_init, hoc_nrnpointerindex, 1);
 _mechtype = nrn_get_mechtype(_mechanism[1]);
     _nrn_setdata_reg(_mechtype, _setdata);
     _nrn_thread_reg(_mechtype, 2, _update_ion_pointer);
 #if NMODL_TEXT
  hoc_reg_nmodl_text(_mechtype, nmodl_file_text);
  hoc_reg_nmodl_filename(_mechtype, nmodl_filename);
#endif
  hoc_register_prop_size(_mechtype, 12, 5);
  hoc_register_dparam_semantics(_mechtype, 0, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "cac_ion");
  hoc_register_dparam_semantics(_mechtype, 4, "cvodeieq");
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 	hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 bkkcasiz C:/Users/ddopp/source/repos/CGProjectpub/modFiles/bkkcasiz.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
static int _reset;
static char *modelname = "";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
 
static int _ode_spec1(_threadargsproto_);
/*static int _ode_matsol1(_threadargsproto_);*/
 static int _slist1[2], _dlist1[2];
 static int states(_threadargsproto_);
 
/*CVODE*/
 static int _ode_spec1 (double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {int _reset = 0; {
   Da = ( ainf ( _threadargscomma_ v , caci ) - a ) * koa ;
   Db = ( binf ( _threadargscomma_ caci ) - b ) * kob ;
   }
 return _reset;
}
 static int _ode_matsol1 (double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {
 Da = Da  / (1. - dt*( ( ( ( - 1.0 ) ) )*( koa ) )) ;
 Db = Db  / (1. - dt*( ( ( ( - 1.0 ) ) )*( kob ) )) ;
  return 0;
}
 /*END CVODE*/
 static int states (double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) { {
    a = a + (1. - exp(dt*(( ( ( - 1.0 ) ) )*( koa ))))*(- ( ( ( ainf ( _threadargscomma_ v , caci ) ) )*( koa ) ) / ( ( ( ( - 1.0 ) ) )*( koa ) ) - a) ;
    b = b + (1. - exp(dt*(( ( ( - 1.0 ) ) )*( kob ))))*(- ( ( ( binf ( _threadargscomma_ caci ) ) )*( kob ) ) / ( ( ( ( - 1.0 ) ) )*( kob ) ) - b) ;
   }
  return 0;
}
 
double ainf ( _threadargsprotocomma_ double _lv , double _lca ) {
   double _lainf;
 _lainf = 1.0 / ( 1.0 + exp ( ( _lv - Vao1 + f * _lca ) / sao1 ) ) * 1.0 / ( 1.0 + exp ( ( _lv - Vao2 + f * _lca ) / sao2 ) ) * ( _lca / ( c1 + _lca ) ) ;
   
return _lainf;
 }
 
static void _hoc_ainf(void) {
  double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   if (_extcall_prop) {_p = _extcall_prop->param; _ppvar = _extcall_prop->dparam;}else{ _p = (double*)0; _ppvar = (Datum*)0; }
  _thread = _extcall_thread;
  _nt = nrn_threads;
 _r =  ainf ( _p, _ppvar, _thread, _nt, *getarg(1) , *getarg(2) );
 hoc_retpushx(_r);
}
 
double binf ( _threadargsprotocomma_ double _lca ) {
   double _lbinf;
 _lbinf = c2 / ( c3 + _lca ) ;
   
return _lbinf;
 }
 
static void _hoc_binf(void) {
  double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   if (_extcall_prop) {_p = _extcall_prop->param; _ppvar = _extcall_prop->dparam;}else{ _p = (double*)0; _ppvar = (Datum*)0; }
  _thread = _extcall_thread;
  _nt = nrn_threads;
 _r =  binf ( _p, _ppvar, _thread, _nt, *getarg(1) );
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
  ek = _ion_ek;
  caci = _ion_caci;
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
  ek = _ion_ek;
  caci = _ion_caci;
 _ode_matsol_instance1(_threadargs_);
 }}
 extern void nrn_update_ion_pointer(Symbol*, Datum*, int, int);
 static void _update_ion_pointer(Datum* _ppvar) {
   nrn_update_ion_pointer(_k_sym, _ppvar, 0, 0);
   nrn_update_ion_pointer(_k_sym, _ppvar, 1, 3);
   nrn_update_ion_pointer(_k_sym, _ppvar, 2, 4);
   nrn_update_ion_pointer(_cac_sym, _ppvar, 3, 1);
 }

static void initmodel(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {
  int _i; double _save;{
  a = a0;
  b = b0;
 {
   a = ainf ( _threadargscomma_ v , caci ) ;
   b = binf ( _threadargscomma_ caci ) ;
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
  caci = _ion_caci;
 initmodel(_p, _ppvar, _thread, _nt);
 }
}

static double _nrn_current(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _v){double _current=0.;v=_v;{ {
   G = g * a * b ;
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
  caci = _ion_caci;
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
  caci = _ion_caci;
 {   states(_p, _ppvar, _thread, _nt);
  } }}

}

static void terminal(){}

static void _initlists(){
 double _x; double* _p = &_x;
 int _i; static int _first = 1;
  if (!_first) return;
 _slist1[0] = &(a) - _p;  _dlist1[0] = &(Da) - _p;
 _slist1[1] = &(b) - _p;  _dlist1[1] = &(Db) - _p;
_first = 0;
}

#if defined(__cplusplus)
} /* extern "C" */
#endif

#if NMODL_TEXT
static const char* nmodl_filename = "bkkcasiz.mod";
static const char* nmodl_file_text = 
  ":BKKCA \n"
  "\n"
  "\n"
  "NEURON {\n"
  "	SUFFIX bkkcasiz\n"
  "	USEION k READ ek WRITE ik\n"
  "	USEION cac READ caci VALENCE 2 \n"
  "	RANGE G, g\n"
  "	RANGE minf, taum, i\n"
  "	RANGE kca\n"
  "}\n"
  "\n"
  "UNITS {\n"
  "	(mA) = (milliamp)\n"
  "	(mV) = (millivolt)\n"
  "}\n"
  "\n"
  "PARAMETER {\n"
  "	g = 0.012 (siemens/cm2) <0,1e9>\n"
  "\n"
  "	f = 0.08    :(mV/uM)\n"
  "	\n"
  "	koa = 0.4  (1/msec)\n"
  "	kob = 0.2 (1/msec)\n"
  "	kCa = 0.36  (1/msec)\n"
  "	\n"
  "	Vao1 =   -5 (mV)\n"
  "	Vao2 = -5 (mV)\n"
  "	\n"
  "	sao1 = -15   :-23 (mV)\n"
  "	sao2 = -9    :-5 (mV)\n"
  "	\n"
  "	c1 = 0.0001  :3.2  : 2.5  :(uM)\n"
  "	c2 =  14  :(uM)\n"
  "	c3 =  15  :(uM)\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "	v (mV)\n"
  "	caci\n"
  "	ek (mV)\n"
  "	i\n"
  "	ik (mA/cm2)\n"
  "	G (siemens/cm2)\n"
  "}\n"
  "\n"
  "STATE {\n"
  "	a b\n"
  "}\n"
  "\n"
  "BREAKPOINT {\n"
  "	SOLVE states METHOD cnexp\n"
  "	G = g*a*b\n"
  "	i = G*(v-ek)\n"
  "	ik = i\n"
  "}\n"
  "\n"
  "INITIAL {\n"
  "	a = ainf(v,caci)\n"
  "	b = binf(caci)\n"
  "}\n"
  "\n"
  "DERIVATIVE states {\n"
  "	a' = (ainf(v,caci)-a)*koa\n"
  "	b' = (binf(caci)-b)*kob\n"
  "}\n"
  "\n"
  "FUNCTION ainf(v(mV),ca) {\n"
  "	ainf = 1/(1+exp((v-Vao1+f*ca)/sao1))*1/(1+exp((v-Vao2+f*ca)/sao2))*(ca/(c1+ca))\n"
  "}\n"
  "\n"
  "FUNCTION binf(ca) {\n"
  "	binf = c2/(c3+ca)\n"
  "}\n"
  ;
#endif
