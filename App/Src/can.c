
#include "can.h"


static CAN_HandleTypeDef hcan;
static void (*can_cb)(uint8_t *);


HAL_StatusTypeDef CAN_Init(void (*cb)(uint8_t *data)){

HAL_StatusTypeDef status;
CAN_FilterTypeDef filter;

    hcan.Instance = CAN_INSTANCE;
    hcan.Init.Prescaler = 60;          // 100kHz
    hcan.Init.Mode = CAN_MODE_LOOPBACK; //CAN_MODE_NORMAL;
    hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan.Init.TimeSeg1 = CAN_BS1_6TQ;
    hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
    hcan.Init.TimeTriggeredMode = DISABLE;
    hcan.Init.AutoBusOff = DISABLE;
    hcan.Init.AutoWakeUp = DISABLE;
    hcan.Init.AutoRetransmission = DISABLE;
    hcan.Init.ReceiveFifoLocked = DISABLE;
    hcan.Init.TransmitFifoPriority = DISABLE;

    status = HAL_CAN_Init(&hcan);

    if ( status != HAL_OK)
    {
      //Error_Handler();
      return status;
    }

    if(cb == NULL){
      //Error_Handler();
      return status;
    }

    can_cb = cb;
   
    filter.FilterBank = 0;
    filter.FilterMode = CAN_FILTERMODE_IDMASK;
    filter.FilterScale = CAN_FILTERSCALE_32BIT;
    filter.FilterIdHigh = 0x0000;
    filter.FilterIdLow = 0x0000;
    filter.FilterMaskIdHigh = 0x0000;
    filter.FilterMaskIdLow = 0x0000;
    filter.FilterFIFOAssignment = CAN_RX_FIFO0;
    filter.FilterActivation = ENABLE;
    filter.SlaveStartFilterBank = 14;

    HAL_CAN_ConfigFilter(&hcan, &filter);
    HAL_CAN_Start(&hcan);

    hcan.Instance->IER |= 2;    

}


/**
 * 
 * */
void CAN_SendTestMessage(void){
uint32_t mailbox;
uint8_t TxData[8];
CAN_TxHeaderTypeDef header;

    header.StdId = 0x11;
    header.RTR = CAN_RTR_DATA;
    header.IDE = CAN_ID_STD;
    header.DLC = 2;
    header.TransmitGlobalTime = DISABLE;
    TxData[0] = 0xCA;
    TxData[1] = 0xFE;

    if(HAL_CAN_AddTxMessage(&hcan, &header, TxData, &mailbox) != HAL_OK){
        return;
    }
    
}


/**
 * 
 * */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
CAN_RxHeaderTypeDef header;
uint8_t rxdata[8];

    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &header, rxdata) != HAL_OK){
        // handle errors
        return;
    }

    if((header.StdId != 0x11)                     ||
     (header.RTR != CAN_RTR_DATA)               ||
     (header.IDE != CAN_ID_STD)                 ||
     (header.DLC != 2)                          ||
     ((rxdata[0]<<8 | rxdata[1]) != 0xCAFE))
  {
    /* Rx message Error */
    return;
  }

    can_cb(rxdata);
}

/**
  * @brief This function handles CAN1 RX0 interrupts.
  */
void CAN1_RX0_IRQHandler(void)
{
  /* USER CODE BEGIN CAN1_RX0_IRQn 0 */

  /* USER CODE END CAN1_RX0_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan);
  /* USER CODE BEGIN CAN1_RX0_IRQn 1 */

  /* USER CODE END CAN1_RX0_IRQn 1 */
}
