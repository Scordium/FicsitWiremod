#include "CircuitryBreadboard.h"

#include "CircuitryBreadboardHologram.h"

void ACircuitryBreadboard::BeginPlay()
{
	Super::BeginPlay();
	auto HologramClass = Cast<ACircuitryBreadboardHologram>(mHologramClass.GetDefaultObject());
	if (!HologramClass) return;
	
	GridSize = HologramClass->SizeStep;
}

FVector ACircuitryBreadboard::GetClosestGridPoint(const FVector& AimPosition)
{
	if (!BoardSize.IsValidSize()) return AimPosition;

	const FVector2D BoardWorldSize = FVector2D(BoardSize.Width * GridSize, BoardSize.Height * GridSize);
	auto BoardPosition = GetClosestBoardPointCoordinates(AimPosition);
	auto BoardSpacePosition = BoardPosition*(GridSize/2);
	auto RelativePosition = BoardSpacePosition - BoardWorldSize/4 + GridSize/4;

	
	auto WorldPosition = GetActorTransform().Inverse().InverseTransformPosition(FVector(RelativePosition, 1));
	
	return WorldPosition;
}

FVector2D ACircuitryBreadboard::GetClosestBoardPointCoordinates(const FVector& AimPosition)
{
	const FVector2D BoardWorldSize = FVector2D(BoardSize.Width * GridSize, BoardSize.Height * GridSize);
	auto UnrotatedAimingPoint = FVector2D(GetActorTransform().InverseTransformPosition(AimPosition)) + BoardWorldSize/4;
	auto BoardPosition = (UnrotatedAimingPoint / (GridSize/2));
	
	return FVector2D((int)BoardPosition.X, (int)BoardPosition.Y);
}
