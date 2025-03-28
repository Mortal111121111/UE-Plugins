// Fill out your copyright notice in the Description page of Project Settings.


#include "ExportDataCommandlet.h"

int32 UExportDataCommandlet::Main(const FString& params)
{
	FString AssetPath;
	FString BoneName;
	FParse::Value(*params, TEXT("-AssetPath="), AssetPath);
	FParse::Value(*params, TEXT("-BoneName="), BoneName);

	UE_LOG(LogTemp, Display, TEXT("UExportDataCommandlet::Main %s %s"),*AssetPath,*BoneName);

	TObjectPtr<UAnimSequence> AnimSequence = LoadObject<UAnimSequence>(this,*ParseAssetPath(AssetPath));
	if (AnimSequence == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UAnimSequence not found"));
		return 0;
	}
	WriteAnimDataToJson(AnimSequence,BoneName);
	Super::Main(params);
	return 0;
}

void UExportDataCommandlet::WriteAnimDataToJson(const UAnimSequence* Sequence,const FString& BoneName)
{
	// 读取数据
	IAnimationDataModel* Model = Sequence->GetDataModel();
	TArray<FTransform> Transforms;
	Model->GetBoneTrackTransforms(FName(BoneName),Transforms);

	// 存储 Json
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField("AssetPaths",Sequence->GetName());
	JsonObject->SetStringField("BoneName",BoneName);

	TArray<TSharedPtr<FJsonValue>> DataObjects;
	for (int i = 0; i < Transforms.Num(); ++i)
	{
		TSharedPtr<FJsonObject> Item = MakeShareable(new FJsonObject);
		Item->SetNumberField("Frames",i);
		FVector Pos = Transforms[i].GetLocation();
		Item->SetNumberField("X",Pos.X);
		Item->SetNumberField("Y",Pos.Y);
		Item->SetNumberField("Z",Pos.Z);
		DataObjects.Add(MakeShareable(new FJsonValueObject(Item)));
	}
	
	JsonObject->SetArrayField("Data",DataObjects);

	// 序列化 JSON
	FString OutputString;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	// 保存到文件
	FString FilePath = FPaths::ProjectSavedDir() / TEXT("PlayerData.json");
	if (FFileHelper::SaveStringToFile(OutputString, *FilePath, FFileHelper::EEncodingOptions::ForceUTF8)) {
		UE_LOG(LogTemp, Display, TEXT("Data saved to: %s"), *FilePath);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Save failed!"));
	}
}

FString UExportDataCommandlet::ParseAssetPath(const FString& AssetPath)
{
	// 1. 获取项目 Content 目录
	FString ContentDir = FPaths::ProjectContentDir();
    
	// 2. 将绝对路径转换为相对 Content 的路径
	FString RelativePath = AssetPath;
	FPaths::MakePathRelativeTo(RelativePath, *ContentDir);

	// 3. 替换反斜杠为斜杠并移除扩展名
	RelativePath.ReplaceInline(TEXT("\\"), TEXT("/"));

	// 4. 拼接为 UE 资源路径格式
	return FString::Printf(TEXT("/Game/%s.%s"), *FPaths::GetBaseFilename(RelativePath,false), *FPaths::GetBaseFilename(RelativePath));
}
