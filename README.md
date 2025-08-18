# plan

small aquarium scene running on baremetal Dreamcast - PVR only, no OpenGL calls

static camera for now, maybe interactive with controller input later

aquarium that takes up the whole screen, no lighting - for now

fish are individual state machines with set behaviors that can cycle at random intervals (or player input)

# fish behavior

```
simple enum {
	SWIMMING
	IDLE
	SLEEPING - BUBBLES
}
```

# fish object

```
components {
	pos x, y, z (vectors)
	vel x, y, z (vectors)
	color (int random)
	state (selected at random from enum)
}
```

# instructions

```
make clean - cleans up the folder

make all - generates an ELF

make test - calls flycast to run the ELF

make disc - generate CDI

make run - calls flycast to run the CDI

make objdump - calls sh-binutils to generate an objdump into a file called 'objdump.asm'. 
```
