#include "DisplayManager.h"

DisplayManager::DisplayManager()
{
	FastLED.addLeds<WS2812B, LED_DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical

	for (uint16_t i = 0; i < NUM_LEDS; i++)
	{
		leds[i] = CRGB::Black;
	}
	for (uint8_t i = 0; i < NUM_DISPLAYS; i++)
	{
		animationManagers[i] = new Animator();
	}
}

DisplayManager::~DisplayManager()
{
}

void DisplayManager::setAllSegmentColors(CRGB color)
{
	for (uint16_t i = 0; i < NUM_SEGMENTS; i++)
	{
		allSegments[i]->updateColor(color);
	}
}

void DisplayManager::setHourSegmentColors(CRGB color)
{
	Displays[0]->updateColor(color);
	Displays[2]->updateColor(color);
}

void DisplayManager::setMinuteSegmentColors(CRGB color)
{
	Displays[4]->updateColor(color);
	Displays[6]->updateColor(color);
}

void DisplayManager::InitSegments(uint16_t indexOfFirstLed, uint8_t ledsPerSegment, CRGB initialColor)
{
	for (uint8_t i = 0; i < NUM_DISPLAYS; i++)
	{
		if(Displays[i] != nullptr)
		{
			free(Displays[i]);
			Displays[i] = nullptr;
		}
	}
	uint16_t currentLEDIndex = indexOfFirstLed;
	for (uint16_t i = 0; i < NUM_SEGMENTS; i++)
	{
		allSegments[i] = new Segment(leds, currentLEDIndex, ledsPerSegment, SegmentDirections[i], initialColor);
		if(Displays[diplayIndex[i]] == nullptr)
		{
			Displays[diplayIndex[i]] = new SevenSegment(SegmentDisplayModes[diplayIndex[i]], animationManagers[diplayIndex[i]]);
		}
		Displays[diplayIndex[i]]->add(allSegments[i], SegmentPositions[i]);
		animationManager.add(allSegments[i]);
		animationManagers[diplayIndex[i]]->add(allSegments[i]);
		currentLEDIndex += ledsPerSegment;
	}
	InitLoadingAnimation(NUM_SEGMENTS_PROGRESS);
}


//TODO: Fix this, it currently crashes
void DisplayManager::displayText(String text)
{
	if(text.length() > 0)
	{
		uint8_t startDisplay;
		if(Displays[0]->canDisplay(text[0]))
		{
			startDisplay = 0;
		} else if(Displays[1]->canDisplay(text[0]))
		{
			startDisplay = 1;
		}
		else
		{
			startDisplay = 2;
		}

		uint8_t textPos = 0;

		do
		{
			if(Displays[startDisplay]->canDisplay(text[textPos]))
			{
				Displays[startDisplay]->DisplayChar(text[textPos]);
			}
			else
			{
				Displays[++startDisplay]->DisplayChar(text[textPos]);
			}
			textPos++;
			startDisplay++;
		}
		while (textPos < text.length() && startDisplay < NUM_DISPLAYS);
	}
}

void DisplayManager::displayTime(uint8_t hours, uint8_t minutes)
{
	if(hours > 24 || minutes > 59)
	{
		Serial.printf("[E] Hours cannot be bigger than 24 but the time was: %02d:%02d\n\r", hours, minutes);
		return;
	}
	
	#if DISPLAY_0_AT_MIDNIGHT == true
		if(hours == 24)
		{
			hours = 0;
		}
	#endif
	#if USE_24_HOUR_FORMAT == false
		if(hours >= 13)
		{
			hours -= 12;
		}
	#endif
	uint8_t firstHourDigit = hours/10;
	if(firstHourDigit == 0 && DISPLAY_SWITCH_OFF_AT_0 == true)
	{
		Displays[0]->off();
	}
	else
	{
		Displays[0]->DisplayNumber(firstHourDigit);
	}
	Displays[2]->DisplayNumber(hours - firstHourDigit * 10); //get the last digit

	uint8_t firstMinuteDigit = minutes/10;
	if(firstMinuteDigit == 0 && DISPLAY_SWITCH_OFF_AT_0 == true)
	{
		Displays[4]->off();
	}
	else
	{
		Displays[4]->DisplayNumber(firstMinuteDigit);
	}
	Displays[6]->DisplayNumber(minutes - firstMinuteDigit * 10); //get the last digit
	// Serial.printf("%d%d:%d%d\n\r", firstHourDigit, hours - firstHourDigit * 10, firstMinuteDigit, minutes - firstMinuteDigit * 10);
}

void DisplayManager::handle()
{
	animationManager.handle();
	for (uint8_t i = 0; i < NUM_DISPLAYS; i++)
	{
		animationManagers[i]->handle();
	}
}

void DisplayManager::setInternalLEDColor(CRGB color)
{
	for (uint16_t i = NUM_LEDS - ADDITIONAL_LEDS; i < NUM_LEDS; i++)
	{
		leds[i] = color;
	}
}

void DisplayManager::AnimationManagersTemporaryOverride(Animator* OverrideanimationManager)
{
	for (uint16_t i = 0; i < NUM_DISPLAYS; i++)
	{
		animationManagerTempBuffer[i] = Displays[i]->AnimationHandler;
		Displays[i]->AnimationHandler = OverrideanimationManager;
	}
}

void DisplayManager::restoreAnimationManagers()
{
	for (uint16_t i = 0; i < NUM_DISPLAYS; i++)
	{
		Displays[i]->AnimationHandler = animationManagerTempBuffer[i];
	}
}

void DisplayManager::showLoadingAnimation()
{	
	// AnimationManagersTemporaryOverride(&animationManager);
	animationManager.PlayComplexAnimation(&LoadingAnimation, true);
}

void DisplayManager::stopLoadingAnimation()
{
	animationManager.ComplexAnimationStopLooping();
}

void DisplayManager::waitForLoadingAnimationFinish()
{
	animationManager.WaitForComplexAnimationCompletion();
	// restoreAnimationManagers();
}

void DisplayManager::turnAllSegmentsOff()
{
	for (uint16_t i = 0; i < NUM_SEGMENTS; i++)
	{
		allSegments[i]->off();
	}
}

void DisplayManager::showProgress(uint32_t progress, uint32_t total)
{
	for (int i = 0; i < (progress / (total / (NUM_LEDS_PER_SEGMENT * NUM_SEGMENTS_PROGRESS))); i++)
	{
		leds[i] = CRGB::Orange;
	}
	animationManager.handle();
}

void DisplayManager::delay(uint32_t timeInMs)
{
	animationManager.delay(timeInMs);
}

void setGlobalBrightness(uint8_t brightness)
{
	FastLED.setBrightness(brightness);
}