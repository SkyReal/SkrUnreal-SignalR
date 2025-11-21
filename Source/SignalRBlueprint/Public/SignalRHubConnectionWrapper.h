/*
 * MIT License
 *
 * Copyright (c) 2020-2022 Frozen Storm Interactive, Yoann Potinet
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include "CoreMinimal.h"
#include "IHubConnection.h"
#include "UObject/Object.h"
#include "SignalRValueWrapper.h"
#include "SignalRHubConnectionWrapper.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnEventReceived, const TArray<FSignalRValueWrapper>&, SignalValues);
DECLARE_DYNAMIC_DELEGATE(FOnHubConnectedEvent);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnHubConnectionErrorEvent, const FString&, Error);
DECLARE_DYNAMIC_DELEGATE(FOnHubConnectionClosedEvent);

struct FSignalRValueWrapper;

UCLASS(BlueprintType, DisplayName = "SignalR Hub Connection")
class SIGNALRBLUEPRINT_API USignalRHubConnectionWrapper : public UObject
{
    GENERATED_BODY()

public:
    void SetHubConnection(const TSharedPtr<IHubConnection>& InHubConnection, const FOnHubConnectedEvent& OnHubConnected,
                          const FOnHubConnectionErrorEvent& OnHubConnectionError,  const FOnHubConnectionClosedEvent& OnHubConnectionClosed);

    UFUNCTION(BlueprintCallable, Category = "SignalR")
    void Start();

    UFUNCTION(BlueprintCallable, Category = "SignalR")
    void Stop();

    UFUNCTION(BlueprintCallable, Category = "SignalR", DisplayName = "Invoke SignalR Method",
        Meta = (AutoCreateRefTerm = "Arguments"))
    void Send(const FString& EventName, const TArray<FSignalRValueWrapper>& Arguments);

    DECLARE_DYNAMIC_DELEGATE_OneParam(FOnInvokeCompleted, const FSignalRInvokeResultWrapper&, Result);

    UFUNCTION(BlueprintCallable, Category = "SignalR", DisplayName = "Invoke SignalR Method (Latent)",
        Meta = (AutoCreateRefTerm = "Arguments, OnCompleted"))
    void Invoke(const FString& EventName, const TArray<FSignalRValueWrapper>& Arguments,
                const FOnInvokeCompleted& OnCompleted);

    UFUNCTION(BlueprintCallable, Category = "SignalR", DisplayName = "Bind Event to SignalR Method")
    void On(const FOnEventReceived& InOnEventReceived, const FString& EventName);

private:
    void OnInvokeCompleted(const FSignalRInvokeResult& Result, FOnInvokeCompleted Delegate);

    TSharedPtr<IHubConnection> HubConnection;
};
