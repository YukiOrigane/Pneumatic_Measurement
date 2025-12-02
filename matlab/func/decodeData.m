%% decode
function x = decodeData(hi, lo)
% hi, lo: uint8
    hi_val = int16(hi) - int16(0x86);
    lo_val = int16(lo) - int16(0x86);
    x = hi_val * 100 + lo_val;
end