/*}
    else if (ovenData.cookmode == 0) {
        if (initial1.state == ADC_CHANGED) {
            if (ovenData.settings_selector == 1) {
                ADCreadingU = ADCreading >> 2;
                ovenData.temperature = ADCreadingU;
                ovenData.temperature += 300;
            }
            else if (ovenData.settings_selector == 0) {
                ADCreadingU = ADCreading >> 2;
                ovenData.initialCookTime = ADCreadingU;
                ovenData.initialCookTime += 1;
                minutes = ovenData.initialCookTime / 60;
                seconds = ovenData.initialCookTime % 60;
            }
        }
        if (ovenData.settings_selector == 0) {
            if (seconds >= 10) {
                sprintf(buffer, "Mode: Bake\n>Time: %d:%d\n Temp: %d%sF", minutes, seconds, ovenData.temperature, DEGREE_SYMBOL);
            }
            else {
                sprintf(buffer, "Mode: Bake\n>Time: %d:0%d\n Temp: %d%sF", minutes, seconds, ovenData.temperature, DEGREE_SYMBOL);
            }
        }
        else if (ovenData.settings_selector == 1) {
            if (seconds >= 10) {
                sprintf(buffer, "Mode: Bake\n Time: %d:%d\n>Temp: %d%sF", minutes, seconds, ovenData.temperature, DEGREE_SYMBOL);
            }
            else {
                sprintf(buffer, "Mode: Bake\n Time: %d:0%d\n>Temp: %d%sF", minutes, seconds, ovenData.temperature, DEGREE_SYMBOL);
            }
        }
    }
    else if (ovenData.cookmode == 1) {
        ovenData.initialCookTime = 1;
        ovenData.settings_selector = 0;
        if (initial1.state == ADC_CHANGED) {
            if (ovenData.settings_selector == 0) {
                ADCreadingU = ADCreading >> 2;
                ovenData.initialCookTime = ADCreadingU;
                ovenData.initialCookTime += 1;
                minutes = ovenData.initialCookTime / 60;
                seconds = ovenData.initialCookTime % 60;
            }
        }
        if (seconds >= 10) {
            sprintf(buffer, "Mode: Toast\nTime: %d:%d\n", minutes, seconds); 
        }
        else {
            sprintf(buffer, "Mode: Toast\nTime: %d:0%d\n", minutes, seconds); 
        }
    }
    else if (ovenData.cookmode == 2) {
        ovenData.temperature = 500;
        ovenData.settings_selector = 0;
        if (initial1.state == ADC_CHANGED) {
            if (ovenData.settings_selector == 0) {
                ADCreadingU = ADCreading >> 2;
                ovenData.initialCookTime = ADCreadingU;
                ovenData.initialCookTime += 1;
                minutes = ovenData.initialCookTime / 60;
                seconds = ovenData.initialCookTime % 60;
            }
        }
        if (seconds >= 10) {
            sprintf(buffer, "Mode: Broil\nTime: %d:%d\nTemp: %d%sF", minutes, seconds, ovenData.temperature, DEGREE_SYMBOL);
        }
        else {
            sprintf(buffer, "Mode: Broil\nTime: %d:0%d\nTemp: %d%sF", minutes, seconds, ovenData.temperature, DEGREE_SYMBOL);
        }
    }*/