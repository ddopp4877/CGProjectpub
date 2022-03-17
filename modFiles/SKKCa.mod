:modified 1/7/2007 by Chris Deister for the GP neuron (to remove some of the background current that existed in Mercer 2007)

NEURON {
	SUFFIX skkca
	USEION cac READ caci VALENCE 2 
	USEION k READ ek WRITE ik 
        RANGE  gbar,gkahp,ik
        GLOBAL inf1,tau1
	GLOBAL Cq101
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
        a0 = 0.0001	(1/ms-mM-mM)	:b0/(1.4e-4^4) 1.3e13 .0002
        b0 = 0.1	(1/ms)			:0.5/(0.100e3), was .5e-2	(1/ms) 0.05	
	        celsius (degC)
	Cq101 = 10
}

STATE {	w }

ASSIGNED {
	ik	(mA/cm2)
        g	(S/cm2)
        inf1
        tau1	(ms)
	a	(1/ms)
        v	(mV)
        ek	(mV)
}

BREAKPOINT {
	SOLVE state METHOD cnexp
	g = gbar*w
	ik = g*(v-ek)
}

INITIAL {
	rate(caci)
	w=inf1
}

DERIVATIVE state {
	rate(caci)
	w' = (inf1 - w)/tau1
}

PROCEDURE rate(caci (mM)) {
	LOCAL q10:, cai_eff
	:if (cai<1e-12) {
	:	cai_eff = 1e-12
	:} else {
	:	cai_eff = cai
	:}
	:q10 = Cq10^((celsius - 22 (degC))/10 (degC) )
	a = a0*(caci)^2
	:a = a0*cai_eff^4
	:tau1 = 100
	tau1 = 4/(a + b0):40/(a + b0)
	inf1 = a/(a + b0)
}