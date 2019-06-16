#!/usr/bin/env python
import subprocess

def twiddle(tol=0.2): 
    p = [2.473301013746727, 0.0, 12.317216854917202] #5.335646378226299, 0.0, 1.3027343200496264 best 3.3788109061711986, 0.25418658283290013, 8.054503064939627. // 3.3788109061711977, 0.0, 7.762011082518108 //2.0688109061711972, 0.0, 6.762011082518107
    dp = [1, 1, 1]
    best_err = estimateError(p)
    while sum(dp) > tol:
        for i in range(3):
            p[i] += dp[i]
            err = estimateError(p)
            if err < best_err:
                best_err = err
                dp[i] *= 1.1
            else:
                p[i] -= 2*dp[i]
                err = estimateError(p)

                if err < best_err:
                    best_err = err
                    dp[i] *= 1.1
                else:
                    p[i] += dp[i]
                    dp[i] *= 0.9
    
    return p, best_err

def estimateError(p):
    print "params: ", p
    out = subprocess.Popen(['./build/pid', str(p[0]), str(p[1]), str(p[2])], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    stdout,stderr = out.communicate()
    print "err: ", float(stdout)
    return float(stdout)

twiddle()  

#431-2.373301013746727, 0.0, 12.417216854917202