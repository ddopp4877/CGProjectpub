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
 
#define nrn_init _nrn_init__nn_old
#define _nrn_initial _nrn_initial__nn_old
#define nrn_cur _nrn_cur__nn_old
#define _nrn_current _nrn_current__nn_old
#define nrn_jacob _nrn_jacob__nn_old
#define nrn_state _nrn_state__nn_old
#define _net_receive _net_receive__nn_old 
#define rate rate__nn_old 
#define state state__nn_old 
 
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
#define g _p[0]
#define inn _p[1]
#define G _p[2]
#define o _p[3]
#define cai _p[4]
#define enn _p[5]
#define Do _p[6]
#define _g _p[7]
#define _ion_cai	*_ppvar[0]._pval
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
 /* declaration of user functions */
 static void _hoc_alp(void);
 static void _hoc_bet(void);
 static void _hoc_exp1(void);
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
 "setdata_nn_old", _hoc_setdata,
 "alp_nn_old", _hoc_alp,
 "bet_nn_old", _hoc_bet,
 "exp1_nn_old", _hoc_exp1,
 "rate_nn_old", _hoc_rate,
 0, 0
};
#define alp alp_nn_old
#define bet bet_nn_old
#define exp1 exp1_nn_old
 extern double alp( double , double );
 extern double bet( double , double );
 extern double exp1( double , double , double );
 /* declare global and static user variables */
#define abar abar_nn_old
 double abar = 0.04;
#define bbar bbar_nn_old
 double bbar = 0.005;
#define celsius1 celsius1_nn_old
 double celsius1 = 25;
#define d2 d2_nn_old
 double d2 = 1;
#define d1 d1_nn_old
 double d1 = 1;
#define k2 k2_nn_old
 double k2 = 100;
#define k1 k1_nn_old
 double k1 = 90;
#define oinf oinf_nn_old
 double oinf = 0;
#define qfact qfact_nn_old
 double qfact = 1;
#define stau stau_nn_old
 double stau = 80;
#define tau tau_nn_old
 double tau = 0;
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 0,0,0
};
 static HocParmUnits _hoc_parm_units[] = {
 "celsius1_nn_old", "degC",
 "abar_nn_old", "/ms",
 "bbar_nn_old", "/ms",
 "tau_nn_old", "ms",
 "g_nn_old", "mho/cm2",
 "inn_nn_old", "mA/cm2",
 "G_nn_old", "mho/cm2",
 0,0
};
 static double delta_t = 0.01;
 static double o0 = 0;
 static double v = 0;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 "stau_nn_old", &stau_nn_old,
 "qfact_nn_old", &qfact_nn_old,
 "celsius1_nn_old", &celsius1_nn_old,
 "d1_nn_old", &d1_nn_old,
 "d2_nn_old", &d2_nn_old,
 "k1_nn_old", &k1_nn_old,
 "k2_nn_old", &k2_nn_old,
 "abar_nn_old", &abar_nn_old,
 "bbar_nn_old", &bbar_nn_old,
 "oinf_nn_old", &oinf_nn_old,
 "tau_nn_old", &tau_nn_old,
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
"nn_old",
 "g_nn_old",
 0,
 "inn_nn_old",
 "G_nn_old",
 0,
 "o_nn_old",
 0,
 0};
 static Symbol* _ca_sym;
 static Symbol* _nn_sym;
 
extern Prop* need_memb(Symbol*);

static void nrn_alloc(Prop* _prop) {
	Prop *prop_ion;
	double *_p; Datum *_ppvar;
 	_p = nrn_prop_data_alloc(_mechtype, 8, _prop);
 	/*initialize range parameters*/
 	g = 0.145;
 	_prop->param = _p;
 	_prop->param_size = 8;
 	_ppvar = nrn_prop_datum_alloc(_mechtype, 5, _prop);
 	_prop->dparam = _ppvar;
 	/*connect ionic variables to this model*/
 prop_ion = need_memb(_ca_sym);
 nrn_promote(prop_ion, 1, 0);
 	_ppvar[0]._pval = &prop_ion->param[1]; /* cai */
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

 void _Nn_old_reg() {
	int _vectorized = 0;
  _initlists();
 	ion_reg("ca", -10000.);
 	ion_reg("nn", 1.0);
 	_ca_sym = hoc_lookup("ca_ion");
 	_nn_sym = hoc_lookup("nn_ion");
 	register_mech(_mechanism, nrn_alloc,nrn_cur, nrn_jacob, nrn_state, nrn_init, hoc_nrnpointerindex, 0);
 _mechtype = nrn_get_mechtype(_mechanism[1]);
     _nrn_setdata_reg(_mechtype, _setdata);
     _nrn_thread_reg(_mechtype, 2, _update_ion_pointer);
 #if NMODL_TEXT
  hoc_reg_nmodl_text(_mechtype, nmodl_file_text);
  hoc_reg_nmodl_filename(_mechtype, nmodl_filename);
#endif
  hoc_register_prop_size(_mechtype, 8, 5);
  hoc_register_dparam_semantics(_mechtype, 0, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "nn_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "nn_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "nn_ion");
  hoc_register_dparam_semantics(_mechtype, 4, "cvodeieq");
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 	hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 nn_old C:/Users/ddopp/source/repos/CGProjectpub/modFiles/Nn_old.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
 
#define FARADAY _nrnunit_FARADAY[_nrnunit_use_legacy_]
static double _nrnunit_FARADAY[2] = {0xc.0f87d73baa128p+3, 96.4853}; /* 96.4853321233100161 */
 
#define R _nrnunit_R[_nrnunit_use_legacy_]
static double _nrnunit_R[2] = {0x8.50809f44c85fp+0, 8.3145}; /* 8.3144626181532395 */
static int _reset;
static char *modelname = "";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
static int rate(double, double);
 
static int _ode_spec1(_threadargsproto_);
/*static int _ode_matsol1(_threadargsproto_);*/
 static int _slist1[1], _dlist1[1];
 static int state(_threadargsproto_);
 
/*CVODE*/
 static int _ode_spec1 () {_reset=0;
 {
   rate ( _threadargscomma_ v , cai ) ;
   Do = ( oinf - o ) / ( tau / qfact ) ;
   }
 return _reset;
}
 static int _ode_matsol1 () {
 rate ( _threadargscomma_ v , cai ) ;
 Do = Do  / (1. - dt*( ( ( ( - 1.0 ) ) ) / ( tau / qfact ) )) ;
  return 0;
}
 /*END CVODE*/
 static int state () {_reset=0;
 {
   rate ( _threadargscomma_ v , cai ) ;
    o = o + (1. - exp(dt*(( ( ( - 1.0 ) ) ) / ( tau / qfact ))))*(- ( ( ( oinf ) ) / ( tau / qfact ) ) / ( ( ( ( - 1.0 ) ) ) / ( tau / qfact ) ) - o) ;
   }
  return 0;
}
 
double alp (  double _lv , double _lca ) {
   double _lalp;
 _lalp = abar / ( 1.0 + exp1 ( _threadargscomma_ k1 , d1 , _lv ) / _lca ) ;
   
return _lalp;
 }
 
static void _hoc_alp(void) {
  double _r;
   _r =  alp (  *getarg(1) , *getarg(2) );
 hoc_retpushx(_r);
}
 
double bet (  double _lv , double _lca ) {
   double _lbet;
 _lbet = bbar / ( 1.0 + _lca / exp1 ( _threadargscomma_ k2 , d2 , _lv ) ) ;
   
return _lbet;
 }
 
static void _hoc_bet(void) {
  double _r;
   _r =  bet (  *getarg(1) , *getarg(2) );
 hoc_retpushx(_r);
}
 
double exp1 (  double _lk , double _ld , double _lv ) {
   double _lexp1;
 _lexp1 = _lk * exp ( - 2.0 * _ld * FARADAY * _lv / R / ( 273.15 + celsius1 ) ) ;
   
return _lexp1;
 }
 
static void _hoc_exp1(void) {
  double _r;
   _r =  exp1 (  *getarg(1) , *getarg(2) , *getarg(3) );
 hoc_retpushx(_r);
}
 
static int  rate (  double _lv , double _lca ) {
   double _la ;
 _la = alp ( _threadargscomma_ _lv , _lca ) ;
   tau = stau / ( _la + bet ( _threadargscomma_ _lv , _lca ) ) ;
   oinf = _la * tau ;
    return 0; }
 
static void _hoc_rate(void) {
  double _r;
   _r = 1.;
 rate (  *getarg(1) , *getarg(2) );
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
  cai = _ion_cai;
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
  cai = _ion_cai;
  enn = _ion_enn;
 _ode_matsol_instance1(_threadargs_);
 }}
 extern void nrn_update_ion_pointer(Symbol*, Datum*, int, int);
 static void _update_ion_pointer(Datum* _ppvar) {
   nrn_update_ion_pointer(_ca_sym, _ppvar, 0, 1);
   nrn_update_ion_pointer(_nn_sym, _ppvar, 1, 0);
   nrn_update_ion_pointer(_nn_sym, _ppvar, 2, 3);
   nrn_update_ion_pointer(_nn_sym, _ppvar, 3, 4);
 }

static void initmodel() {
  int _i; double _save;_ninits++;
 _save = t;
 t = 0.0;
{
  o = o0;
 {
   rate ( _threadargscomma_ v , cai ) ;
   o = oinf ;
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
  cai = _ion_cai;
  enn = _ion_enn;
 initmodel();
 }}

static double _nrn_current(double _v){double _current=0.;v=_v;{ {
   G = g * o ;
   inn = g * o * ( v - enn ) ;
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
  cai = _ion_cai;
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
  cai = _ion_cai;
  enn = _ion_enn;
 { error =  state();
 if(error){fprintf(stderr,"at line 59 in file Nn_old.mod:\n	SOLVE state METHOD cnexp\n"); nrn_complain(_p); abort_run(error);}
 } }}

}

static void terminal(){}

static void _initlists() {
 int _i; static int _first = 1;
  if (!_first) return;
 _slist1[0] = &(o) - _p;  _dlist1[0] = &(Do) - _p;
_first = 0;
}

#if NMODL_TEXT
static const char* nmodl_filename = "Nn_old.mod";
static const char* nmodl_file_text = 
  ": Calcium activated K channel.\n"
  ": From Moczydlowski and Latorre (1983) J. Gen. Physiol. 82\n"
  ": Model 3. (Scheme R1 page 523)\n"
  ": Steephen JE, Manchanda R (2009) Differences in biophysical properties \n"
  ": of nucleus accumbens medium spiny neurons emerging from \n"
  ": inactivation of inward rectifying potassium currents. \n"
  ": J Comput Neurosci  ModelDB\n"
  "\n"
  "\n"
  "\n"
  "UNITS {\n"
  "	(molar) = (1/liter)\n"
  "	(mV) =	(millivolt)\n"
  "	(mA) =	(milliamp)\n"
  "	FARADAY = (faraday)  (kilocoulombs)\n"
  "	R = (k-mole) (joule/degC)\n"
  "}\n"
  "\n"
  "NEURON {\n"
  "	SUFFIX nn_old\n"
  "	USEION ca READ cai\n"
  "	USEION nn READ enn WRITE inn VALENCE 1\n"
  "	RANGE G, inn, gmax, g\n"
  "	GLOBAL oinf, tau\n"
  "}\n"
  "\n"
  "PARAMETER {\n"
  "	stau = 80\n"
  "	qfact = 1\n"
  "	celsius1	= 25	(degC) : 35\n"
  "	v		(mV)\n"
  ":	gmax=0.175	(mho/cm2)	: Maximum Permeability \n"
  "	g=0.145	(mho/cm2)	: Maximum Permeability Default\n"
  "	\n"
  ":	gmax=0.2	(mho/cm2)	: Maximum Permeability\n"
  "	\n"
  "	cai		:(uM) \n"
  "\n"
  "\n"
  "	d1 =  1                 :.84	      :page 527 Table II channel A\n"
  "	d2 = 1.0			:our index 2 is the paper's subscript 4\n"
  "	k1 =90	:(uM)\n"
  "	k2 = 100                 :.011	(mM)\n"
  "	abar = .04	(/ms)      :.48\n"
  "	bbar = .005	(/ms) :page 524. our bbar is the paper's alpha\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "	inn		(mA/cm2)\n"
  "	oinf\n"
  "	enn (mV)\n"
  "	tau		(ms)\n"
  "	G		(mho/cm2)\n"
  "}\n"
  "\n"
  "STATE {	o }		: fraction of open channels\n"
  "\n"
  "BREAKPOINT {\n"
  "	SOLVE state METHOD cnexp\n"
  "	G = g*o\n"
  "	inn = g*o*(v - enn)\n"
  "}\n"
  "\n"
  "DERIVATIVE state {\n"
  "	rate(v, cai)\n"
  "	o' = (oinf - o)/(tau/qfact)\n"
  "}\n"
  "\n"
  "INITIAL {\n"
  "	rate(v, cai)\n"
  "	o = oinf\n"
  ":	VERBATIM\n"
  ":		printf(\"R = %f\\n\",R);\n"
  ":		printf(\"F = %f\\n\",FARADAY);\n"
  ":	ENDVERBATIM\n"
  "}\n"
  "\n"
  ": From R1 page 523. beta in the paper is the rate from closed to open\n"
  ": and we call it alp here.\n"
  "\n"
  "FUNCTION alp(v (mV), ca (uM)) (1/ms) { :callable from hoc\n"
  "	alp = abar/(1 + exp1(k1,d1,v)/ca)\n"
  "}\n"
  "\n"
  "FUNCTION bet(v (mV), ca (uM)) (1/ms) { :callable from hoc\n"
  "	bet = bbar/(1 + ca/exp1(k2,d2,v))\n"
  "}\n"
  "\n"
  "FUNCTION exp1(k (uM), d, v (mV)) (uM) { :callable from hoc\n"
  "	exp1 = k*exp(-2*d*FARADAY*v/R/(273.15 + celsius1))\n"
  "}\n"
  "\n"
  "PROCEDURE rate(v (mV), ca (uM)) { :callable from hoc\n"
  "	LOCAL a\n"
  "	a = alp(v,ca)\n"
  "	tau = stau/(a + bet(v, ca))\n"
  "	oinf = a*tau\n"
  "}\n"
  ;
#endif
