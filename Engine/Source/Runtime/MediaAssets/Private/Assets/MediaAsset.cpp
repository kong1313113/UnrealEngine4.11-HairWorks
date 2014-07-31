// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "MediaAssetsPrivatePCH.h"


/* UMediaAsset structors
 *****************************************************************************/

UMediaAsset::UMediaAsset( const class FPostConstructInitializeProperties& PCIP )
	: Super(PCIP)
	, AutoPlay(false)
	, Looping(true)
	, PlaybackRate(1.0f)
	, ResetOnLastFrame(false)
	, StreamMode(MASM_FromMemory)
	, MediaPlayer(nullptr)
{ }


/* UMediaAsset interface
 *****************************************************************************/

bool UMediaAsset::CanPause( ) const
{
	return MediaPlayer.IsValid() && MediaPlayer->IsPlaying();
}


bool UMediaAsset::CanPlay( ) const
{
	return MediaPlayer.IsValid() && !MediaPlayer->IsPlaying() && MediaPlayer->IsReady();
}


bool UMediaAsset::CanStop( ) const
{
	return MediaPlayer.IsValid() && MediaPlayer->IsReady();
}


FTimespan UMediaAsset::GetDuration( ) const
{
	if (MediaPlayer.IsValid())
	{
		return MediaPlayer->GetDuration();
	}

	return FTimespan::Zero();
}


float UMediaAsset::GetRate( ) const
{
	if (MediaPlayer.IsValid())
	{
		return MediaPlayer->GetRate();
	}

	return 0;
}


FTimespan UMediaAsset::GetTime( ) const
{
	if (MediaPlayer.IsValid())
	{
		return MediaPlayer->GetTime();
	}

	return FTimespan::Zero();
}


bool UMediaAsset::IsPaused( ) const
{
	return MediaPlayer.IsValid() && MediaPlayer->IsPaused();
}


bool UMediaAsset::IsPlaying( ) const
{
	return MediaPlayer.IsValid() && MediaPlayer->IsPlaying();
}


bool UMediaAsset::IsStopped( ) const
{
	return !MediaPlayer.IsValid() || !MediaPlayer->IsReady();
}


bool UMediaAsset::Pause( )
{
	return MediaPlayer.IsValid() && MediaPlayer->Pause();
}


bool UMediaAsset::Rewind( )
{
	return MediaPlayer.IsValid() && MediaPlayer->Seek(FTimespan::Zero());
}


bool UMediaAsset::Play( )
{
	return MediaPlayer.IsValid() && MediaPlayer->Play();
}


bool UMediaAsset::SetRate( float InRate )
{
	PlaybackRate = InRate;

	if (!MediaPlayer.IsValid() || !MediaPlayer->IsPlaying())
	{
		return false;
	}

	return MediaPlayer->Play(InRate);
}


bool UMediaAsset::Seek( const FTimespan& InTime )
{
	return MediaPlayer.IsValid() && MediaPlayer->Seek(InTime);
}


void UMediaAsset::Stop( )
{
	if (MediaPlayer.IsValid())
	{
		Rewind();
		Pause();
	}
}


bool UMediaAsset::SupportsRate( float Rate, bool Unthinned ) const
{
	return MediaPlayer.IsValid() && MediaPlayer->SupportsRate(Rate, Unthinned);
}


bool UMediaAsset::SupportsScrubbing( ) const
{
	return MediaPlayer.IsValid() && MediaPlayer->SupportsScrubbing();
}


bool UMediaAsset::SupportsSeeking( ) const
{
	return MediaPlayer.IsValid() && MediaPlayer->SupportsSeeking();
}


/* UObject  overrides
 *****************************************************************************/

void UMediaAsset::BeginDestroy( )
{
	Super::BeginDestroy();

	if (MediaPlayer.IsValid())
	{
		MediaPlayer->Close();
		MediaPlayer.Reset();
	}
}


FString UMediaAsset::GetDesc( )
{
	if (MediaPlayer.IsValid())
	{
		return TEXT("UMediaAsset");
	}

	return FString();
}


void UMediaAsset::PostLoad( )
{
	Super::PostLoad();

	if (!HasAnyFlags(RF_ClassDefaultObject) && !GIsBuildMachine)
	{
		InitializePlayer();
	}
}


#if WITH_EDITOR

void UMediaAsset::PostEditChangeProperty( FPropertyChangedEvent& PropertyChangedEvent )
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	InitializePlayer();
}

#endif


/* UMediaAsset implementation
 *****************************************************************************/

void UMediaAsset::InitializePlayer( )
{
	const FString& NewUrl = URL.FilePath;

	if (NewUrl != CurrentUrl)
	{
		// close previous player
		CurrentUrl = FString();

		if (MediaPlayer.IsValid())
		{
			MediaPlayer->Close();
			MediaPlayer->OnClosing().RemoveAll(this);
			MediaPlayer->OnOpened().RemoveAll(this);
			MediaPlayer.Reset();
		}

		if (NewUrl.IsEmpty())
		{
			return;
		}

		// create new player
		MediaPlayer = IMediaModule::Get().CreatePlayer(NewUrl);

		if (!MediaPlayer.IsValid())
		{
			return;
		}

		MediaPlayer->OnClosing().AddUObject(this, &UMediaAsset::HandleMediaPlayerMediaClosing);
		MediaPlayer->OnOpened().AddUObject(this, &UMediaAsset::HandleMediaPlayerMediaOpened);

		// open the new media file
		bool OpenedSuccessfully = false;

		if (StreamMode == EMediaAssetStreamModes::MASM_FromUrl)
		{
			OpenedSuccessfully = MediaPlayer->Open(NewUrl);
		}
		else
		{
			FArchive* FileReader = IFileManager::Get().CreateFileReader(*NewUrl);
		
			if (FileReader == nullptr)
			{
				return;
			}

			if (FileReader->TotalSize() > 0)
			{
				TArray<uint8>* FileData = new TArray<uint8>();

				FileData->AddUninitialized(FileReader->TotalSize());
				FileReader->Serialize(FileData->GetData(), FileReader->TotalSize());

				OpenedSuccessfully = MediaPlayer->Open(MakeShareable(FileData), NewUrl);
			}

			delete FileReader;
		}

		// finish initialization
		if (OpenedSuccessfully)
		{
			CurrentUrl = NewUrl;
		}
	}

	if (!MediaPlayer.IsValid())
	{
		return;
	}
    
	if (AutoPlay)
	{
		MediaPlayer->Play();
	}
}


/* UMediaAsset callbacks
 *****************************************************************************/

void UMediaAsset::HandleMediaPlayerMediaClosing( FString ClosedUrl )
{
	OnMediaClosing.Broadcast(ClosedUrl);
}


void UMediaAsset::HandleMediaPlayerMediaOpened( FString OpenedUrl )
{
	OnMediaOpened.Broadcast(OpenedUrl);
}
