#include "iev.h"
#include "fatfs.h"

char SDPath[4];   /* SD logical drive path */
FATFS SDFatFS;    /* File system object for SD logical drive */

void SDCard::fatFs_Init(void){
FRESULT fr;
FIL fo;       /* File object for SD */
UINT wbytes; /* File write counts */
char fname[] = "STM32.TXT";
uint8_t wtext[] = "text to write logical disk"; /* File write buffer */

	if(FATFS_LinkDriver(&SD_Driver, SDPath) != 0)
	{
		console->print("FATFS Link Driver fail\n");
		return;
	}

	fr = f_mount(&SDFatFS, (TCHAR const*)SDPath, 1);
	
	if(fr != FR_OK){
		console->print("FATFS Fail to mount: %x\n", fr);
		return;
	}

	console->print("File system: %x\n", SDFatFS.fs_type);
 	
	if(f_open(&fo, fname, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
	{
		if(f_write(&fo, wtext, sizeof(wtext), &wbytes) == FR_OK);
		{
			f_close(&fo);
		}
	} 
 	FATFS_UnLinkDriver(SDPath);
}

void SDCard::help(void){}

char SDCard::execute(void *ptr) {
	uint32_t operation = 0;
	FRESULT fr;

	char *p1, c;
	
	p1 = (char*)ptr;
	
		// check parameters
	if( p1 == NULL || *p1 == '\0'){
		help();
		return CMD_OK;        
	}

	if(BSP_SD_IsDetected() != SD_PRESENT){
		console->print("SD card not inserted\n");
		return CMD_OK;
	}

     while( !(operation & OPT_DONE) ){
        c = nextChar(&p1);
        switch(c){
			case 's':
				BSP_SD_Init();
				break;

			case 'i':
				HAL_SD_CardInfoTypeDef ci;
				BSP_SD_GetCardInfo(&ci);
				console->print("\nType: %x\n", ci.CardType);
				console->print("Version: %x\n", ci.CardVersion);
				console->print("Class: %x\n", ci.Class);
				console->print("Relative address: %x\n", ci.RelCardAdd);
				console->print("Number of blocks: %x, (%d)\n", ci.BlockNbr, ci.BlockNbr);
				console->print("Block Size: %d\n", ci.BlockSize);
				console->print("Logical Number of blocks: %x, (%d)\n", ci.LogBlockNbr, ci.LogBlockNbr);
				console->print("Logical Block Size: %d\n\n", ci.LogBlockSize);
				break;

				
            case 'm':				
				fatFs_Init();				
            	return CMD_OK;

            case '\n':
            case '\r':    
            case '\0':
                OPT_SET_FLAG(operation, OPT_DONE);
        }
    }

    switch(OPT_OPER(operation)){
        
        default:
            //console->print("Not implemented\n");
            break;
    }
    
	return CMD_OK;
}