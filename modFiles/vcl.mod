: ham-fisted voltage clamp

NEURON {
	POINT_PROCESS vcl
	NONSPECIFIC_CURRENT ivcl
	RANGE vh,vc,vr,t1,dur,kp,kd,ki,err,derr,ierr,on
}


PARAMETER {

	on = 1

	kp = 2000
	kd = 100
	ki = 300
	
	t1 = 100
	dur = 100
	
	vh = -80
	vc = 0
	vr = -80

}

ASSIGNED {
	v 
	ivcl
	err
	errlast

}

STATE {

	ierr
	
}

INITIAL {
	ierr = 0
}


BREAKPOINT {
	SOLVE states METHOD cnexp

	if(on) {	
	if(t<t1){err = vh-v
	}else if(t<t1+dur) {err = vc-v
	}else {err = vr-v
	}
	}else { err = 0
	}
		
	ivcl = -(kp*err + kd*(err-errlast) + ki*ierr)

}

DERIVATIVE states {

	ierr' = err

}