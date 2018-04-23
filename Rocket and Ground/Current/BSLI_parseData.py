def parseData (data):
        startFlag = 0x7E
        endFlag = 0x81
        escapeFlag = 0x99
        finalData = data
        errorArray = bytearray(b'error')
        #de-encapsulate
        # convert from immutable "bytes" object to mutable "bytearray" object
        # read byte by byte, remove escape bytes
        # put into object type for transmission (bytes or bytearray)


        #check for the start and end tags
        if (finalData[0] != startFlag) or (finalData[len(finalData) - 1] != endFlag):
            #error
            return errorArray
        else:
            #delete start and end tags
            del finalData[0]
            del finalData[len(finalData) - 1]


            i = 0
            print(finalData)
            
            #go through the bytearray 
            while i < len(finalData):
                print(i)
                print(hex(finalData[i]))
                if finalData[i] == escapeFlag:
                    #check that the escape flag actually escapes a proper character
                    if (i == len(finalData) - 1) or ((finalData[i + 1] != startFlag) \
                       and (finalData[i + 1] != endFlag) and (finalData[i + 1] != escapeFlag)):
                        #error
                        print("escape doesnt escape anything")
                        return errorArray

                    else:
                        #remove escape flag
                        del finalData[i]
                        

                elif (finalData[i] == startFlag) or (finalData[i] == endFlag):
                    #error
                    print("no escape flag")
                    return errorArray
                
                i = i + 1
                print(finalData)
                
        return finalData


if __name__ == "__main__":
        sf = 0x7e
        end = 0x81
        esc = 0x99
        print(parseData(bytearray([sf, 0x12, esc, esc, esc, sf, 0x12, end])))
