// Fill out your copyright notice in the Description page of Project Settings.


#include "SkipNumberText.h"

#include "MovieScene.h"
#include "Animation/MovieScene2DTransformSection.h"
#include "Animation/MovieScene2DTransformTrack.h"
#include "Animation/WidgetAnimation.h"
#include "Components/PanelWidget.h"

void USkipNumberText::NativeConstruct()
{
    Super::NativeConstruct();
}

void USkipNumberText::InitSkipNumber( int32 InNumberValue )
{
    NumberValue = InNumberValue;
    TxtNumberText->SetText( FText::AsNumber( InNumberValue ) );
    BuildAnim( TxtNumberText );
}

void USkipNumberText::AddSkipNumber( int32 InNumberValue )
{
    NumberValue += InNumberValue;
    TxtNumberText->SetText( FText::AsNumber( NumberValue ) );
}

void USkipNumberText::StartUpdateSkipNumber( int32 InChangedValue )
{
    EndUpdateSkipNumberText();

    UpdateValue = FMath::Floor( InChangedValue / SkipCounts );
    if(UpdateValue < FloatMaxValue)
    {
        AddSkipNumber( InChangedValue );
        return;
    }

    UpdateCurrentValue = NumberValue;
    NumberValue += InChangedValue;
    float Rate = UpdateTotalTime / SkipCounts;

    FTimerManager& TimerManager = GetWorld()->GetTimerManager();
    TimerManager.SetTimer( UpdateTimerHandle, this, &USkipNumberText::UpdateSkipNumberText, Rate, true );
    PlayAnimation( SimpleAnim );
}


void USkipNumberText::UpdateSkipNumberText()
{
    UpdateCurrentValue += UpdateValue - FMath::RandRange( 0, FloatMaxValue );
    if(UpdateCurrentValue >= NumberValue)
    {
        EndUpdateSkipNumberText();
        return;
    }

    TxtNumberText->SetText( FText::AsNumber( UpdateCurrentValue ) );
}

void USkipNumberText::EndUpdateSkipNumberText()
{
    FTimerManager& TimerManager = GetWorld()->GetTimerManager();
    TimerManager.ClearTimer( UpdateTimerHandle );
    TxtNumberText->SetText( FText::AsNumber( NumberValue ) );
}

void USkipNumberText::BuildAnim( UWidget* BindingObject )
{
    SimpleAnim = NewObject<UWidgetAnimation>( this, FName(), RF_Transactional );
    if(SimpleAnim)
    {
        FString AnimName = "SimpleAnim";
        //MyAnim->SetDisplayLabel(AnimName);
        SimpleAnim->MovieScene = NewObject<UMovieScene>( SimpleAnim, FName( *AnimName ), RF_Transactional );
        // Default to 20 fps display rate (as was the previous default in USequencerSettings)
        SimpleAnim->MovieScene->SetDisplayRate( FFrameRate( 20, 1 ) );

        // 设置动画时长 第一帧和最后一帧
        const FFrameTime InFrame = 0.0f * SimpleAnim->MovieScene->GetTickResolution();
        const FFrameTime OutFrame = AnimTime * SimpleAnim->MovieScene->GetTickResolution();
        SimpleAnim->MovieScene->SetPlaybackRange( TRange<FFrameNumber>( InFrame.FrameNumber, OutFrame.FrameNumber + 1 ) );

        const FGuid PossessableGuid = SimpleAnim->MovieScene->AddPossessable( *BindingObject->GetName(), BindingObject->GetClass() );

        //add 2DTransform track
        TObjectPtr<UMovieScene2DTransformTrack> TransformTrack = SimpleAnim->MovieScene->AddTrack<UMovieScene2DTransformTrack>( PossessableGuid );
        if(TransformTrack)
        {
            // 设置轨道名
            TransformTrack->SetPropertyNameAndPath( "RenderTransform", "RenderTransform" );

            // 创建 Section
            UMovieScene2DTransformSection* TransformSection = Cast<UMovieScene2DTransformSection>( TransformTrack->CreateNewSection() );
            TransformSection->SetRange( TRange<FFrameNumber>( InFrame.FrameNumber, OutFrame.FrameNumber + 1 ) ); //full sequencer

            // 在 Section 上增加 key 点位
            for(const FVector2D& Key : AnimKeys)
            {
                AddKeyToChannel( &TransformSection->Translation[0], ( Key.X * SimpleAnim->MovieScene->GetTickResolution() ).FrameNumber, Key.Y, EMovieSceneKeyInterpolation::Auto );
                AddKeyToChannel( &TransformSection->Translation[1], ( Key.X * SimpleAnim->MovieScene->GetTickResolution() ).FrameNumber, Key.Y, EMovieSceneKeyInterpolation::Auto );
            }

            // 将编辑好的 Section 添加到 Track 上
            TransformTrack->AddSection( *TransformSection );
        }
        //create binding
        SimpleAnim->BindPossessableObject( PossessableGuid, *BindingObject, this );
    }
}