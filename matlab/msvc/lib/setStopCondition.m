function setStopCondition(epsg, epsf, epsx, maxits)
    if ~libisloaded('optimizer')
        loadlibrary('optimizer', 'optimizer.hpp'); % msvc
    end
    
    if libisloaded('optimizer')
        libname = 'optimizer';
    end
    
    calllib(libname, 'setStopCondition', epsg, epsf, epsx, maxits);
end

