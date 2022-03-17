NEURON {
	POINT_PROCESS ExpSynCG
	RANGE tau, e, i, initW, W, G 
	NONSPECIFIC_CURRENT i
}

UNITS {
	(nA) = (nanoamp)
	(mV) = (millivolt)
	(uS) = (microsiemens)
}

PARAMETER {
	tau = 0.1 (ms) <1e-9,1e9>
	e = 0	(mV)
	initW = 1
	G = 1
}

ASSIGNED {
	v (mV)
	i (nA)
	W
	t0 (ms)
}

STATE {
	g (uS)
}

INITIAL {
		W = initW
		t0 = -1
}

BREAKPOINT {
	SOLVE state METHOD cnexp

}

DERIVATIVE state {
	g' = -g/tau
	G = g * W
	i = G*(v - e)
}

NET_RECEIVE(dummy_weight) {
	t0=t
	:g = g + weight
}