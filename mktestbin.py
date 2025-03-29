with open('out', 'wb') as f:
    # magic
    f.write(b'\xFE\xFA\xFA\xFC')
    # 8 bytes in data
    f.write(b'\x00\x08')
    # literal 5
    f.write(b'\x00\x00\x00\x05')
    # literal 8
    f.write(b'\x00\x00\x00\x03')
    # instructions
