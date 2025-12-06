package main

import (
	"fmt"
	"time"
)

func sendMessages(ch chan string) {
	messages := []string{"Hello", "World", "From", "Golang"}
	for _, msg := range messages {
		ch <- msg // Send message to the channel
		fmt.Println("Sent:", msg)
		time.Sleep(1 * time.Second) // Simulate some work
	}
	close(ch) // Close the channel when done
}

func main() {
	// Create a channel
	ch := make(chan string)

	// Start a goroutine to send messages
	go sendMessages(ch)

	// Receive messages from the channel
	for msg := range ch {
		fmt.Println("Received:", msg)
	}

	fmt.Println("All messages received!")
}