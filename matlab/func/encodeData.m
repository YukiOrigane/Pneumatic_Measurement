%% encode
function [hi, lo] = encodeData(x)
% x: int16 value (-10000 ~ 10000)
% 出力: hi(上位), lo(下位)

    % MATLAB の mod は負数時の挙動が C と異なるので調整
    lo_val = rem(x, 100);
    hi_val = (x - lo_val) / 100;

    hi = uint8(hi_val + 0x86);
    lo = uint8(lo_val + 0x86);

    % 符号一致チェック
    if (hi_val > 0 && lo_val < 0) || (hi_val < 0 && lo_val > 0)
        error("符号不一致: エンコードエラー");
    end
end