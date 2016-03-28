function setStopCondition(epsg, epsf, epsx, maxits)
    if ~libisloaded('optimizer') && ~libisloaded('liboptimizer')
        try
            loadlibrary('optimizer', 'optimizer.hpp'); % msvc
        catch
            loadlibrary('liboptimizer', 'optimizer.hpp'); % mingw
        end
    end
    
    if libisloaded('optimizer')
        libname = 'optimizer';
    elseif libisloaded('liboptimizer')
        libname = 'liboptimizer';   
    end
    
    calllib(libname, 'setStopCondition', epsg, epsf, epsx, maxits);
end

