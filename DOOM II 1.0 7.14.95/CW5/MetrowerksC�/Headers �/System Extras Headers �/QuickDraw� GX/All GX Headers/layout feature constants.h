/*	File:		layout feature constants.h	Copyright:	� 1984-1994 by Apple Computer, Inc., all rights reserved.	WARNING	This file was auto generated by the interfacer tool. Modifications	must be made to the master file.*/#ifndef layoutFeatureConstantsIncludes#define layoutFeatureConstantsIncludes	#ifdef __cplusplusextern "C" {	#endifenum  {	allTypographicFeaturesType	= 0,	allTypeFeaturesOnSelector	= 0,	allTypeFeaturesOffSelector	= 1,	ligaturesType				= 1,	requiredLigaturesOnSelector	= 0,	requiredLigaturesOffSelector = 1,	commonLigaturesOnSelector	= 2,	commonLigaturesOffSelector	= 3,	rareLigaturesOnSelector		= 4,	rareLigaturesOffSelector	= 5,	logosOnSelector				= 6,	logosOffSelector			= 7,	rebusPicturesOnSelector		= 8,	rebusPicturesOffSelector	= 9,	diphthongLigaturesOnSelector = 10,	diphthongLigaturesOffSelector = 11,	cursiveConnectionType		= 2,	unconnectedSelector			= 0,	partiallyConnectedSelector	= 1,	cursiveSelector				= 2,	letterCaseType				= 3,	upperAndLowerCaseSelector	= 0,	allCapsSelector				= 1,	allLowerCaseSelector		= 2,	smallCapsSelector			= 3,	initialCapsSelector			= 4,	initialCapsAndSmallCapsSelector = 5,	verticalSubstitutionType	= 4,	substituteVerticalFormsOnSelector = 0,	substituteVerticalFormsOffSelector = 1,	linguisticRearrangementType	= 5,	linguisticRearrangementOnSelector = 0,	linguisticRearrangementOffSelector = 1,	numberSpacingType			= 6,	monospacedNumbersSelector	= 0,	proportionalNumbersSelector	= 1,	appleReserved1Type			= 7,	smartSwashType				= 8,	wordInitialSwashesOnSelector = 0,	wordInitialSwashesOffSelector = 1,	wordFinalSwashesOnSelector	= 2,	wordFinalSwashesOffSelector	= 3,	lineInitialSwashesOnSelector = 4,	lineInitialSwashesOffSelector = 5,	lineFinalSwashesOnSelector	= 6,	lineFinalSwashesOffSelector	= 7,	nonFinalSwashesOnSelector	= 8,	nonFinalSwashesOffSelector	= 9,	diacriticsType				= 9,	showDiacriticsSelector		= 0,	hideDiacriticsSelector		= 1,	decomposeDiacriticsSelector	= 2,	verticalPositionType		= 10,	normalPositionSelector		= 0,	superiorsSelector			= 1,	inferiorsSelector			= 2,	ordinalsSelector			= 3,	fractionsType				= 11,	noFractionsSelector			= 0,	verticalFractionsSelector	= 1,	diagonalFractionsSelector	= 2,	appleReserved2Type			= 12,	overlappingCharactersType	= 13,	preventOverlapOnSelector	= 0,	preventOverlapOffSelector	= 1,	typographicExtrasType		= 14,	hyphensToEmDashOnSelector	= 0,	hyphensToEmDashOffSelector	= 1,	hyphenToEnDashOnSelector	= 2,	hyphenToEnDashOffSelector	= 3,	unslashedZeroOnSelector		= 4,	unslashedZeroOffSelector	= 5,	formInterrobangOnSelector	= 6,	formInterrobangOffSelector	= 7,	smartQuotesOnSelector		= 8,	smartQuotesOffSelector		= 9,	mathematicalExtrasType		= 15,	hyphenToMinusOnSelector		= 0,	hyphenToMinusOffSelector	= 1,	asteriskToMultiplyOnSelector = 2,	asteriskToMultiplyOffSelector = 3,	slashToDivideOnSelector		= 4,	slashToDivideOffSelector	= 5,	inequalityLigaturesOnSelector = 6,	inequalityLigaturesOffSelector = 7,	exponentsOnSelector			= 8,	exponentsOffSelector		= 9,	ornamentSetsType			= 16,	noOrnamentsSelector			= 0,	dingbatsSelector			= 1,	piCharactersSelector		= 2,	fleuronsSelector			= 3,	decorativeBordersSelector	= 4,	internationalSymbolsSelector = 5,	mathSymbolsSelector			= 6,	characterAlternativesType	= 17,	noAlternatesSelector		= 0,	designComplexityType		= 18,	designLevel1Selector		= 0,	designLevel2Selector		= 1,	designLevel3Selector		= 2,	designLevel4Selector		= 3,	designLevel5Selector		= 4,	styleOptionsType			= 19,	noStyleOptionsSelector		= 0,	displayTextSelector			= 1,	engravedTextSelector		= 2,	illuminatedCapsSelector		= 3,	titlingCapsSelector			= 4,	tallCapsSelector			= 5,	characterShapeType			= 20,	traditionalCharactersSelector = 0,	simplifiedCharactersSelector = 1,	numberCaseType				= 21,	lowerCaseNumbersSelector	= 0,	upperCaseNumbersSelector	= 1,	lastFeatureType				= -1};enum  {	typographicFeaturesType		= allTypographicFeaturesType,	enableEffectsOnSelector		= allTypeFeaturesOnSelector,	enableEffectsOffSelector	= allTypeFeaturesOffSelector,	ligatureType				= ligaturesType,	ligatureRequiredOnSelector	= requiredLigaturesOnSelector,	ligatureRequiredOffSelector	= requiredLigaturesOffSelector,	ligatureCommonOnSelector	= commonLigaturesOnSelector,	ligatureCommonOffSelector	= commonLigaturesOffSelector,	ligatureRareOnSelector		= rareLigaturesOnSelector,	ligatureRareOffSelector		= rareLigaturesOffSelector,	ligatureLogotypeOnSelector	= logosOnSelector,	ligatureLogotypeOffSelector	= logosOffSelector,	ligatureRebusOnSelector		= rebusPicturesOnSelector,	ligatureRebusOffSelector	= rebusPicturesOffSelector,	cursiveConnectionNoneSelector = unconnectedSelector,	cursiveConnectionPartialSelector = partiallyConnectedSelector,	cursiveConnectionFullSelector = cursiveSelector,	letterNumberCaseType		= letterCaseType,	letterNumberCaseUpperLowerSelector = upperAndLowerCaseSelector,	letterNumberCaseAllUpperSelector = allCapsSelector,	letterNumberCaseAllLowerSelector = allLowerCaseSelector,	letterNumberCaseSmallCapsSelector = smallCapsSelector,	letterNumberCaseUpperThenLowerSelector = initialCapsSelector,	letterNumberCaseUpperThenSmCapsSelector = initialCapsAndSmallCapsSelector,	verticalSubstitutionOnSelector = substituteVerticalFormsOnSelector,	verticalSubstitutionOffSelector = substituteVerticalFormsOffSelector,	rearrangementType			= linguisticRearrangementType,	rearrangementOnSelector		= linguisticRearrangementOnSelector,	rearrangementOffSelector	= linguisticRearrangementOffSelector,	numberStyleType				= numberSpacingType,	numberStyleForColumnsSelector = monospacedNumbersSelector,	numberStyleProportionalSelector = proportionalNumbersSelector,	smartSwashWordInitialsOnSelector = wordInitialSwashesOnSelector,	smartSwashWordInitialsOffSelector = wordInitialSwashesOffSelector,	smartSwashWordFinalsOnSelector = wordFinalSwashesOnSelector,	smartSwashWordFinalsOffSelector = wordFinalSwashesOffSelector,	smartSwashLineInitialsOnSelector = lineInitialSwashesOnSelector,	smartSwashLineInitialsOffSelector = lineInitialSwashesOffSelector,	smartSwashLineFinalsOnSelector = lineFinalSwashesOnSelector,	smartSwashLineFinalsOffSelector = lineFinalSwashesOffSelector,	smartSwashArchaicNonFinalsOnSelector = nonFinalSwashesOnSelector,	smartSwashArchaicNonFinalsOffSelector = nonFinalSwashesOffSelector,	diacriticsShowSelector		= showDiacriticsSelector,	diacriticsHideSelector		= hideDiacriticsSelector,	diacriticsSerializeSelector	= decomposeDiacriticsSelector,	verticalPositionNormalSelector = normalPositionSelector,	verticalPositionSuperiorsSelector = superiorsSelector,	verticalPositionInferiorsSelector = inferiorsSelector,	verticalPositionOrdinalsSelector = ordinalsSelector,	fractionsDontFormSelector	= noFractionsSelector,	fractionsFormVerticalSelector = verticalFractionsSelector,	fractionsFormDiagonalSelector = diagonalFractionsSelector,	diphthongLigaturesType		= ligaturesType,	overlappingGlyphsType		= overlappingCharactersType,	overlappingGlyphsPreventOnSelector = preventOverlapOnSelector,	overlappingGlyphsPreventOffSelector = preventOverlapOffSelector,	extrasHyphensToEmDashOnSelector = hyphensToEmDashOnSelector,	extrasHyphensToEmDashOffSelector = hyphensToEmDashOffSelector,	extrasHyphenToEnDashOnSelector = hyphenToEnDashOnSelector,	extrasHyphenToEnDashOffSelector = hyphenToEnDashOffSelector,	extrasForceUnslashedZeroOnSelector = unslashedZeroOnSelector,	extrasForceUnslashedZeroOffSelector = unslashedZeroOffSelector,	extrasMakeInterrobangOnSelector = formInterrobangOnSelector,	extrasMakeInterrobangOffSelector = formInterrobangOffSelector,	mathExtrasType				= mathematicalExtrasType,	mathHyphenToMinusOnSelector	= hyphenToMinusOnSelector,	mathHyphenToMinusOffSelector = hyphenToMinusOffSelector,	mathStarToMultiplyOnSelector = asteriskToMultiplyOnSelector,	mathStarToMultiplyOffSelector = asteriskToMultiplyOffSelector,	mathSlashToDivideOnSelector	= slashToDivideOnSelector,	mathSlashToDivideOffSelector = slashToDivideOffSelector,	mathInequalityLigaturesOnSelector = inequalityLigaturesOnSelector,	mathInequalityLigaturesOffSelector = inequalityLigaturesOffSelector,	mathExponentiationOnSelector = exponentsOnSelector,	mathExponentiationOffSelector = exponentsOffSelector,	ornamentSetsNoneSelector	= noOrnamentsSelector,	ornamentSetsDingbatsSelector = dingbatsSelector,	ornamentSetsPiCharactersSelector = piCharactersSelector,	ornamentSetsFleuronsSelector = fleuronsSelector,	ornamentSetsDecorativeBordersSelector = decorativeBordersSelector,	ornamentSetsIntlSymbolsSelector = internationalSymbolsSelector,	ornamentSetsMathSymbolsSelector = mathSymbolsSelector,	glyphAlternativesType		= characterAlternativesType};	#ifdef __cplusplus}	#endif#endif