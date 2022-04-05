function jointWaypoints = interpolationJointAngles(currentAngles,desiredAngles, ...
    currentTMatrix, desiredTMatrix)
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here


currentAngles = [currentAngles(2) currentAngles(4) currentAngles(7) ...
    currentAngles(12) currentAngles(14)];

desiredAngles = [desiredAngles(2) desiredAngles(4) desiredAngles(7) ...
    desiredAngles(12) desiredAngles(14)];

timeStep = 0.1; % seconds
toolSpeed = 0.1; % m/s

distance = norm(tform2trvec(currentTMatrix)-tform2trvec(desiredTMatrix));
initTime = 0;
finalTime = (distance/toolSpeed) - initTime;
trajTimes = initTime:timeStep:finalTime;
timeInterval = [trajTimes(1); trajTimes(end)];

ctrlpoints = [currentAngles', desiredAngles'];
jointConfigArray = cubicpolytraj(ctrlpoints,timeInterval,trajTimes);
jointWaypoints = bsplinepolytraj(jointConfigArray,timeInterval,1);
end

