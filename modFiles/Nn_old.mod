: Calcium activated K channel.
: From Moczydlowski and Latorre (1983) J. Gen. Physiol. 82
: Model 3. (Scheme R1 page 523)
: Steephen JE, Manchanda R (2009) Differences in biophysical properties 
: of nucleus accumbens medium spiny neurons emerging from 
: inactivation of inward rectifying potassium currents. 
: J Comput Neurosci  ModelDB



UNITS {
	(molar) = (1/liter)
	(mV) =	(millivolt)
	(mA) =	(milliamp)
	FARADAY = (faraday)  (kilocoulombs)
	R = (k-mole) (joule/degC)
}

NEURON {
	SUFFIX nn_old
	USEION ca READ cai
	USEION nn READ enn WRITE inn VALENCE 1
	RANGE G, inn, gmax, g
	GLOBAL oinf, tau
}

PARAMETER {
	stau = 80
	qfact = 1
	celsius1	= 25	(degC) : 35
	v		(mV)
:	gmax=0.175	(mho/cm2)	: Maximum Permeability 
	g=0.145	(mho/cm2)	: Maximum Permeability Default
	
:	gmax=0.2	(mho/cm2)	: Maximum Permeability
	
	cai		:(uM) 


	d1 =  1                 :.84	      :page 527 Table II channel A
	d2 = 1.0			:our index 2 is the paper's subscript 4
	k1 =90	:(uM)
	k2 = 100                 :.011	(mM)
	abar = .04	(/ms)      :.48
	bbar = .005	(/ms) :page 524. our bbar is the paper's alpha
}

ASSIGNED {
	inn		(mA/cm2)
	oinf
	enn (mV)
	tau		(ms)
	G		(mho/cm2)
}

STATE {	o }		: fraction of open channels

BREAKPOINT {
	SOLVE state METHOD cnexp
	G = g*o
	inn = g*o*(v - enn)
}

DERIVATIVE state {
	rate(v, cai)
	o' = (oinf - o)/(tau/qfact)
}

INITIAL {
	rate(v, cai)
	o = oinf
:	VERBATIM
:		printf("R = %f\n",R);
:		printf("F = %f\n",FARADAY);
:	ENDVERBATIM
}

: From R1 page 523. beta in the paper is the rate from closed to open
: and we call it alp here.

FUNCTION alp(v (mV), ca (uM)) (1/ms) { :callable from hoc
	alp = abar/(1 + exp1(k1,d1,v)/ca)
}

FUNCTION bet(v (mV), ca (uM)) (1/ms) { :callable from hoc
	bet = bbar/(1 + ca/exp1(k2,d2,v))
}

FUNCTION exp1(k (uM), d, v (mV)) (uM) { :callable from hoc
	exp1 = k*exp(-2*d*FARADAY*v/R/(273.15 + celsius1))
}

PROCEDURE rate(v (mV), ca (uM)) { :callable from hoc
	LOCAL a
	a = alp(v,ca)
	tau = stau/(a + bet(v, ca))
	oinf = a*tau
}
