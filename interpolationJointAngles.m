function [jointWaypoints,jointConfigArray] = interpolationJointAngles(currentAngles,desiredAngles, ...
    currentTMatrix, desiredTMatrix)

timeStep = 1; % seconds
toolSpeed = 0.5; % m/s

distance = norm(tform2trvec(currentTMatrix)-tform2trvec(desiredTMatrix));
initTime = 0;
finalTime = (distance/toolSpeed) - initTime;
trajTimes = initTime:timeStep:finalTime;
timeInterval = [trajTimes(1); trajTimes(end)];

ctrlpoints = [currentAngles, desiredAngles];
jointConfigArray = cubicpolytraj(ctrlpoints,timeInterval,trajTimes);
jointWaypoints = bsplinepolytraj(jointConfigArray,timeInterval,1);

end

