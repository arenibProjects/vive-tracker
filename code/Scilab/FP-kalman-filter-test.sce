// All units are in millimeters, seconds
clc
clear

function [P]=trajectory(t)
    // Circle
    r = 1000
    omega = 1
    C(1) = 1500
    C(2) = 1000
    
    P(1,:) = C(1)+r*cos(omega*t)
    P(2,:) = C(2)+r*sin(omega*t)
    P(3,:) = (%pi/2) + modulo
endfunction

t = 0:0.001:10
truth = trajectory(t)
plot2d(truth(1,:), truth(2,:), 2)

function [P]=sensors(truth)
    for i=1:2:12
        P(i,:) = truth()
    end
endfunction
