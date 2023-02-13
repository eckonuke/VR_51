// Fill out your copyright notice in the Description page of Project Settings.


#include "MovieWidget.h"
#include <UMG/Public/Components/Button.h>
#include "MyMovieActor.h"

void UMovieWidget::NativeConstruct() {
	btn_play->OnPressed.AddDynamic(this, &UMovieWidget::Play);
	btn_stop->OnPressed.AddDynamic(this, &UMovieWidget::Stop);
	btn_reverse->OnPressed.AddDynamic(this, &UMovieWidget::Reverse);
	btn_forward->OnPressed.AddDynamic(this, &UMovieWidget::Forward);
}

void UMovieWidget::Play()
{
	movieActor->PlayMovie();
}

void UMovieWidget::Stop()
{
	movieActor->StopMovie();
}

void UMovieWidget::Reverse()
{
	movieActor->ReverseMovie(5);
}

void UMovieWidget::Forward()
{
	movieActor->ForwardMovie(5);
}
