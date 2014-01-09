function [ Checksum ] = doChecksum( frame )
%doChkSum calculate the checksum of the frame
%   0xFF - uint8(bitand(sum(frame), hex2dec('FF')))
Checksum = uint8(hex2dec('FF')) - uint8(bitand(sum(frame), hex2dec('FF')));
    


end

