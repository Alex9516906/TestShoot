// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define LOG_PRETTY(LogCategory, LogVerbosity, Format, ...) \
UE_LOG(LogCategory, LogVerbosity, TEXT("[%s] " Format), ANSI_TO_TCHAR(__FUNCTION__), ##__VA_ARGS__)

DECLARE_LOG_CATEGORY_EXTERN(UILog, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(NetworkLog, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(PCMenuLog, Log, All);

