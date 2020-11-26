// Fill out your copyright notice in the Description page of Project Settings.

#include "Boid.h"


// Sets default values
ABoid::ABoid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereRadius = 51.f;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>("myMesh");
	RootComponent = mesh;

	collisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("My Sphere Component"));
	collisionSphere->InitSphereRadius(sphereRadius);
	collisionSphere->SetCollisionProfileName("Trigger");

	collisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ABoid::OnOverlapBegin);
	OnActorHit.AddDynamic(this, &ABoid::OnHit);
}

// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();
	pastPosition = GetActorLocation();
	initializeCollisionParams();
	sensoric = new SideAndGroundSensoric(collisionParams);
}

// Called every frame
void ABoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->drawDebugSphere)
		DrawDebugSphere(GetWorld(), GetActorLocation(), sphereRadius, 10, FColor::Purple, false, -1, 0, 1);
}

void ABoid::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent*
	OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
		if (OtherActor->GetClass() != this->GetClass())
			deactivate();
	}
}

void ABoid::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (SelfActor->GetClass() != OtherActor->GetClass()) {
		isInAir = false;
	}
}

void ABoid::resetVelocity()
{
	mesh->SetPhysicsLinearVelocity(FVector(0, 0, 0));
	mesh->SetSimulatePhysics(true);
}

void ABoid::activate()
{
	this->active = true;
}

void ABoid::deactivate()
{
	this->active = false;
	DisableComponentsSimulatePhysics();
}

void ABoid::initializeCollisionParams()
{
	TArray<ABoid*> actors;
	FindAllActors(GetWorld(), actors);

	for (int i = 0; i < actors.Num(); i++)
		collisionParams.AddIgnoredActor(actors[i]);
}

void ABoid::move()
{
	FVector newLocation = GetActorLocation();
	FRotator newRotation = GetActorRotation();
	FVector currentVelocity = GetVelocity();
	FVector newVelocity = FVector(0, 0, currentVelocity.Z);

	FVector zVector = FVector(0.f, 0.f, 1.f);
	FVector forwardVector = GetActorForwardVector();
	zVector.Normalize(0.f);

	if (forward)
		newVelocity += GetActorForwardVector() * velocity;
	if (backwards)
		newVelocity -= GetActorForwardVector() * velocity;
	if (left) 
		newVelocity += FVector::CrossProduct(forwardVector, zVector) * velocity;
	if (right)
		newVelocity -= FVector::CrossProduct(forwardVector, zVector) * velocity;

	newVelocity.Normalize();
	newVelocity *= velocity;
	newVelocity.Z = currentVelocity.Z;
	if (jump) {
		if (!isInAir)
			newVelocity.Z = (jumpVelocity);
	}

	if (rotateCounterClockwise) 
		newRotation.Yaw += 5.f;
	if (rotateClockwise)
		newRotation.Yaw -= 5.f;

	mesh->SetPhysicsLinearVelocity(newVelocity, false);
	SetActorRotation(newRotation);
	isInAir = true;
}

void ABoid::setPastPosition(FVector position)
{
	pastPosition = position;
}

FVector ABoid::getPastPosition()
{
	return pastPosition;
}

bool ABoid::getActive()
{
	return active;
}

void ABoid::setId(int id)
{
	this->id = id;
}

void ABoid::iterate()
{
	checkIfActive();
	if (active) 
		move();
}

void ABoid::checkIfActive()
{
	if (GetActorLocation().Z < -50) {
		DisableComponentsSimulatePhysics();
		active = false;
	}
}

void ABoid::updateSensorData()
{
	sensorDataForNeuralNet.clear();
	sensorDataForNeuralNet = sensoric->getDataFromSensors(GetActorForwardVector(), GetActorLocation(), GetWorld());
}

vector<double> ABoid::getSensorData()
{
	return sensorDataForNeuralNet;
}

void ABoid::updateMoveDirection(vector<double> neuralNetOutput)
{
	forward = neuralNetOutput[0] > 0.5f ? true : false;
	backwards = neuralNetOutput[1] > 0.5f ? true : false;
	left = neuralNetOutput[2] > 0.5f ? true : false;
	right = neuralNetOutput[3] > 0.5f ? true : false;
	rotateClockwise = neuralNetOutput[4] > 0.5f ? true : false;
	rotateCounterClockwise = neuralNetOutput[5] > 0.5f ? true : false;
	jump = neuralNetOutput[6] > 0.5f ? true : false;
}