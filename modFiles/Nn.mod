:modified 1/7/2007 by Chris Deister for the GP neuron (to remove some of the background current that existed in Mercer 2007)

NEURON {
	SUFFIX Nn
	USEION cac READ caci VALENCE 2 
	USEION nn READ enn WRITE inn VALENCE 1
        RANGE  gbar,gkahp,inn
        GLOBAL inf,tau
	GLOBAL Cq10
}

UNITS {
	(mA) = (milliamp)
	(mV) = (millivolt)
	(molar) = (1/liter)
	(mM) = (millimolar)
	(S) = (siemens)
}

PARAMETER {
	gbar = 1	(S/cm2)
        n = 4
        caci	(mM)
        a0 = .0002	(1/ms-mM)	:b0/(1.4e-4^4) 1.3e13
        b0 = 0.05	(1/ms)			:0.5/(0.100e3), was .5e-2	(1/ms)	
	        celsius (degC)
	Cq10 = 10
}

STATE {	w }

ASSIGNED {
	inn	(mA/cm2)
        g	(S/cm2)
        inf
        tau	(ms)
	a	(1/ms)
        v	(mV)
        enn	(mV)
}

BREAKPOINT {
	SOLVE state METHOD cnexp
	g = gbar*w
	inn = g*(v-enn)
}

INITIAL {
	rate(caci)
	w=inf
}

DERIVATIVE state {
	rate(caci)
	w' = (inf - w)/tau
}

PROCEDURE rate(caci (mM)) {
	LOCAL q10:, cai_eff
	:if (cai<1e-12) {
	:	cai_eff = 1e-12
	:} else {
	:	cai_eff = cai
	:}
	:q10 = Cq10^((celsius - 22 (degC))/10 (degC) )
	a = a0*caci^2
	:a = a0*cai_eff^4
	:tau = 1000
	tau = .4/(a + b0)
	:tau = 5*(a + b0)
	inf = a/(a + b0)
}