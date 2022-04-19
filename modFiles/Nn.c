/* Created by Language version: 7.7.0 */
/* NOT VECTORIZED */
#define NRN_VECTORIZED 0
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
 
#define nrn_init _nrn_init__Nn
#define _nrn_initial _nrn_initial__Nn
#define nrn_cur _nrn_cur__Nn
#define _nrn_current _nrn_current__Nn
#define nrn_jacob _nrn_jacob__Nn
#define nrn_state _nrn_state__Nn
#define _net_receive _net_receive__Nn 
#define rate rate__Nn 
#define state state__Nn 
 
#define _threadargscomma_ /**/
#define _threadargsprotocomma_ /**/
#define _threadargs_ /**/
#define _threadargsproto_ /**/
 	/*SUPPRESS 761*/
	/*SUPPRESS 762*/
	/*SUPPRESS 763*/
	/*SUPPRESS 765*/
	 extern double *getarg();
 static double *_p; static Datum *_ppvar;
 
#define t nrn_threads->_t
#define dt nrn_threads->_dt
#define gbar _p[0]
#define inn _p[1]
#define w _p[2]
#define caci _p[3]
#define Dw _p[4]
#define g _p[5]
#define a _p[6]
#define enn _p[7]
#define _g _p[8]
#define _ion_caci	*_ppvar[0]._pval
#define _ion_enn	*_ppvar[1]._pval
#define _ion_inn	*_ppvar[2]._pval
#define _ion_dinndv	*_ppvar[3]._pval
 
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
 /* external NEURON variables */
 extern double celsius;
 /* declaration of user functions */
 static void _hoc_rate(void);
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
 _p = _prop->param; _ppvar = _prop->dparam;
 }
 static void _hoc_setdata() {
 Prop *_prop, *hoc_getdata_range(int);
 _prop = hoc_getdata_range(_mechtype);
   _setdata(_prop);
 hoc_retpushx(1.);
}
 /* connect user functions to hoc names */
 static VoidFunc hoc_intfunc[] = {
 "setdata_Nn", _hoc_setdata,
 "rate_Nn", _hoc_rate,
 0, 0
};
 /* declare global and static user variables */
#define Cq10 Cq10_Nn
 double Cq10 = 10;
#define a0 a0_Nn
 double a0 = 0.0002;
#define b0 b0_Nn
 double b0 = 0.05;
#define inf inf_Nn
 double inf = 0;
#define n n_Nn
 double n = 4;
#define tau tau_Nn
 double tau = 0;
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 0,0,0
};
 static HocParmUnits _hoc_parm_units[] = {
 "a0_Nn", "1/ms-mM",
 "b0_Nn", "1/ms",
 "tau_Nn", "ms",
 "gbar_Nn", "S/cm2",
 "inn_Nn", "mA/cm2",
 0,0
};
 static double delta_t = 0.01;
 static double v = 0;
 static double w0 = 0;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 "n_Nn", &n_Nn,
 "a0_Nn", &a0_Nn,
 "b0_Nn", &b0_Nn,
 "Cq10_Nn", &Cq10_Nn,
 "inf_Nn", &inf_Nn,
 "tau_Nn", &tau_Nn,
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
"Nn",
 "gbar_Nn",
 0,
 "inn_Nn",
 0,
 "w_Nn",
 0,
 0};
 static Symbol* _cac_sym;
 static Symbol* _nn_sym;
 
extern Prop* need_memb(Symbol*);

static void nrn_alloc(Prop* _prop) {
	Prop *prop_ion;
	double *_p; Datum *_ppvar;
 	_p = nrn_prop_data_alloc(_mechtype, 9, _prop);
 	/*initialize range parameters*/
 	gbar = 1;
 	_prop->param = _p;
 	_prop->param_size = 9;
 	_ppvar = nrn_prop_datum_alloc(_mechtype, 5, _prop);
 	_prop->dparam = _ppvar;
 	/*connect ionic variables to this model*/
 prop_ion = need_memb(_cac_sym);
 nrn_promote(prop_ion, 1, 0);
 	_ppvar[0]._pval = &prop_ion->param[1]; /* caci */
 prop_ion = need_memb(_nn_sym);
 nrn_promote(prop_ion, 0, 1);
 	_ppvar[1]._pval = &prop_ion->param[0]; /* enn */
 	_ppvar[2]._pval = &prop_ion->param[3]; /* inn */
 	_ppvar[3]._pval = &prop_ion->param[4]; /* _ion_dinndv */
 
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

 void _Nn_reg() {
	int _vectorized = 0;
  _initlists();
 	ion_reg("cac", 2.0);
 	ion_reg("nn", 1.0);
 	_cac_sym = hoc_lookup("cac_ion");
 	_nn_sym = hoc_lookup("nn_ion");
 	register_mech(_mechanism, nrn_alloc,nrn_cur, nrn_jacob, nrn_state, nrn_init, hoc_nrnpointerindex, 0);
 _mechtype = nrn_get_mechtype(_mechanism[1]);
     _nrn_setdata_reg(_mechtype, _setdata);
     _nrn_thread_reg(_mechtype, 2, _update_ion_pointer);
 #if NMODL_TEXT
  hoc_reg_nmodl_text(_mechtype, nmodl_file_text);
  hoc_reg_nmodl_filename(_mechtype, nmodl_filename);
#endif
  hoc_register_prop_size(_mechtype, 9, 5);
  hoc_register_dparam_semantics(_mechtype, 0, "cac_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "nn_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "nn_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "nn_ion");
  hoc_register_dparam_semantics(_mechtype, 4, "cvodeieq");
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 	hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 Nn C:/Users/ddopp/source/repos/CGProjectpub/modFiles/Nn.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
static int _reset;
static char *modelname = "";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
static int rate(double);
 
static int _ode_spec1(_threadargsproto_);
/*static int _ode_matsol1(_threadargsproto_);*/
 static int _slist1[1], _dlist1[1];
 static int state(_threadargsproto_);
 
/*CVODE*/
 static int _ode_spec1 () {_reset=0;
 {
   rate ( _threadargscomma_ caci ) ;
   Dw = ( inf - w ) / tau ;
   }
 return _reset;
}
 static int _ode_matsol1 () {
 rate ( _threadargscomma_ caci ) ;
 Dw = Dw  / (1. - dt*( ( ( ( - 1.0 ) ) ) / tau )) ;
  return 0;
}
 /*END CVODE*/
 static int state () {_reset=0;
 {
   rate ( _threadargscomma_ caci ) ;
    w = w + (1. - exp(dt*(( ( ( - 1.0 ) ) ) / tau)))*(- ( ( ( inf ) ) / tau ) / ( ( ( ( - 1.0 ) ) ) / tau ) - w) ;
   }
  return 0;
}
 
static int  rate (  double _lcaci ) {
   double _lq10 ;
 a = a0 * pow( _lcaci , 2.0 ) ;
   tau = .4 / ( a + b0 ) ;
   inf = a / ( a + b0 ) ;
    return 0; }
 
static void _hoc_rate(void) {
  double _r;
   _r = 1.;
 rate (  *getarg(1) );
 hoc_retpushx(_r);
}
 
static int _ode_count(int _type){ return 1;}
 
static void _ode_spec(_NrnThread* _nt, _Memb_list* _ml, int _type) {
   Datum* _thread;
   Node* _nd; double _v; int _iml, _cntml;
  _cntml = _ml->_nodecount;
  _thread = _ml->_thread;
  for (_iml = 0; _iml < _cntml; ++_iml) {
    _p = _ml->_data[_iml]; _ppvar = _ml->_pdata[_iml];
    _nd = _ml->_nodelist[_iml];
    v = NODEV(_nd);
  caci = _ion_caci;
  enn = _ion_enn;
     _ode_spec1 ();
  }}
 
static void _ode_map(int _ieq, double** _pv, double** _pvdot, double* _pp, Datum* _ppd, double* _atol, int _type) { 
 	int _i; _p = _pp; _ppvar = _ppd;
	_cvode_ieq = _ieq;
	for (_i=0; _i < 1; ++_i) {
		_pv[_i] = _pp + _slist1[_i];  _pvdot[_i] = _pp + _dlist1[_i];
		_cvode_abstol(_atollist, _atol, _i);
	}
 }
 
static void _ode_matsol_instance1(_threadargsproto_) {
 _ode_matsol1 ();
 }
 
static void _ode_matsol(_NrnThread* _nt, _Memb_list* _ml, int _type) {
   Datum* _thread;
   Node* _nd; double _v; int _iml, _cntml;
  _cntml = _ml->_nodecount;
  _thread = _ml->_thread;
  for (_iml = 0; _iml < _cntml; ++_iml) {
    _p = _ml->_data[_iml]; _ppvar = _ml->_pdata[_iml];
    _nd = _ml->_nodelist[_iml];
    v = NODEV(_nd);
  caci = _ion_caci;
  enn = _ion_enn;
 _ode_matsol_instance1(_threadargs_);
 }}
 extern void nrn_update_ion_pointer(Symbol*, Datum*, int, int);
 static void _update_ion_pointer(Datum* _ppvar) {
   nrn_update_ion_pointer(_cac_sym, _ppvar, 0, 1);
   nrn_update_ion_pointer(_nn_sym, _ppvar, 1, 0);
   nrn_update_ion_pointer(_nn_sym, _ppvar, 2, 3);
   nrn_update_ion_pointer(_nn_sym, _ppvar, 3, 4);
 }

static void initmodel() {
  int _i; double _save;_ninits++;
 _save = t;
 t = 0.0;
{
  w = w0;
 {
   rate ( _threadargscomma_ caci ) ;
   w = inf ;
   }
  _sav_indep = t; t = _save;

}
}

static void nrn_init(_NrnThread* _nt, _Memb_list* _ml, int _type){
Node *_nd; double _v; int* _ni; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
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
  caci = _ion_caci;
  enn = _ion_enn;
 initmodel();
 }}

static double _nrn_current(double _v){double _current=0.;v=_v;{ {
   g = gbar * w ;
   inn = g * ( v - enn ) ;
   }
 _current += inn;

} return _current;
}

static void nrn_cur(_NrnThread* _nt, _Memb_list* _ml, int _type){
Node *_nd; int* _ni; double _rhs, _v; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
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
  caci = _ion_caci;
  enn = _ion_enn;
 _g = _nrn_current(_v + .001);
 	{ double _dinn;
  _dinn = inn;
 _rhs = _nrn_current(_v);
  _ion_dinndv += (_dinn - inn)/.001 ;
 	}
 _g = (_g - _rhs)/.001;
  _ion_inn += inn ;
#if CACHEVEC
  if (use_cachevec) {
	VEC_RHS(_ni[_iml]) -= _rhs;
  }else
#endif
  {
	NODERHS(_nd) -= _rhs;
  }
 
}}

static void nrn_jacob(_NrnThread* _nt, _Memb_list* _ml, int _type){
Node *_nd; int* _ni; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
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
 
}}

static void nrn_state(_NrnThread* _nt, _Memb_list* _ml, int _type){
Node *_nd; double _v = 0.0; int* _ni; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
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
  caci = _ion_caci;
  enn = _ion_enn;
 { error =  state();
 if(error){fprintf(stderr,"at line 43 in file Nn.mod:\n	SOLVE state METHOD cnexp\n"); nrn_complain(_p); abort_run(error);}
 } }}

}

static void terminal(){}

static void _initlists() {
 int _i; static int _first = 1;
  if (!_first) return;
 _slist1[0] = &(w) - _p;  _dlist1[0] = &(Dw) - _p;
_first = 0;
}

#if NMODL_TEXT
static const char* nmodl_filename = "Nn.mod";
static const char* nmodl_file_text = 
  ":modified 1/7/2007 by Chris Deister for the GP neuron (to remove some of the background current that existed in Mercer 2007)\n"
  "\n"
  "NEURON {\n"
  "	SUFFIX Nn\n"
  "	USEION cac READ caci VALENCE 2 \n"
  "	USEION nn READ enn WRITE inn VALENCE 1\n"
  "        RANGE  gbar,gkahp,inn\n"
  "        GLOBAL inf,tau\n"
  "	GLOBAL Cq10\n"
  "}\n"
  "\n"
  "UNITS {\n"
  "	(mA) = (milliamp)\n"
  "	(mV) = (millivolt)\n"
  "	(molar) = (1/liter)\n"
  "	(mM) = (millimolar)\n"
  "	(S) = (siemens)\n"
  "}\n"
  "\n"
  "PARAMETER {\n"
  "	gbar = 1	(S/cm2)\n"
  "        n = 4\n"
  "        caci	(mM)\n"
  "        a0 = .0002	(1/ms-mM)	:b0/(1.4e-4^4) 1.3e13\n"
  "        b0 = 0.05	(1/ms)			:0.5/(0.100e3), was .5e-2	(1/ms)	\n"
  "	        celsius (degC)\n"
  "	Cq10 = 10\n"
  "}\n"
  "\n"
  "STATE {	w }\n"
  "\n"
  "ASSIGNED {\n"
  "	inn	(mA/cm2)\n"
  "        g	(S/cm2)\n"
  "        inf\n"
  "        tau	(ms)\n"
  "	a	(1/ms)\n"
  "        v	(mV)\n"
  "        enn	(mV)\n"
  "}\n"
  "\n"
  "BREAKPOINT {\n"
  "	SOLVE state METHOD cnexp\n"
  "	g = gbar*w\n"
  "	inn = g*(v-enn)\n"
  "}\n"
  "\n"
  "INITIAL {\n"
  "	rate(caci)\n"
  "	w=inf\n"
  "}\n"
  "\n"
  "DERIVATIVE state {\n"
  "	rate(caci)\n"
  "	w' = (inf - w)/tau\n"
  "}\n"
  "\n"
  "PROCEDURE rate(caci (mM)) {\n"
  "	LOCAL q10:, cai_eff\n"
  "	:if (cai<1e-12) {\n"
  "	:	cai_eff = 1e-12\n"
  "	:} else {\n"
  "	:	cai_eff = cai\n"
  "	:}\n"
  "	:q10 = Cq10^((celsius - 22 (degC))/10 (degC) )\n"
  "	a = a0*caci^2\n"
  "	:a = a0*cai_eff^4\n"
  "	:tau = 1000\n"
  "	tau = .4/(a + b0)\n"
  "	:tau = 5*(a + b0)\n"
  "	inf = a/(a + b0)\n"
  "}\n"
  ;
#endif
