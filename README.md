# Systems final project - Nala Sharadjaya, Xinhui Xu

### How-to
1. Compile: `$ make`
1. Run `$ ./server` and up to as many `$ ./client` as you want from different terminal sessions.
	* `$ ./client [ip]` for manual ip input 
2. In `client`, create a new user by typing in whatever username you want!
3. Create a new project by following the prompts (then create multiple if you want!). Then view your projects following the prompts.
4. You can view your projects, enter them, and view your tasks. Other features vary in functionality...if you don't see a response you expect, you can type `>> home` to be escorted back to safety. Otherwise `CTRL+C` and start over!

### Things that will work on a scale of probably to definitely
* Creating a new user (def)
* Creating a new proj (def)
* Viewing all the tasks (prob?)
* Viewing all YOUR tasks (def)

### Things we didn't have time to properly integrate
* Adding members and tasks 
* Editing status of tasks
* Removing things (tasks, projects, users)

### Things we did :)
* Set up an efficient and intuitive keyboard-only user-interface. Using numbers to tell the server where to go next requires forcing the server to pay close attention to where it is right now. When the control flow isn't linear, things got trickier exponentially.
* Permissions when accessing projects and tasks. Figuring out who can access what where and when, setting up the directory structure (which had to be revamped many times).
* Utilization of default C libraries to get things such as file manipulation, string wizadry, etc. done the basic but sturdy way. 

### `users/` structure

Note: you can either create your own new user when running `client.c` for the first time or use an existing user, see below.

```
user1/
	aardvark/
	baboon/
	camel/
user2/
	dog/
	elephant/
	flamingo/	
user3/
	giraffe/
	hippo/
	iguana/
```

Projects a particular user manages are listed as directories within that user's directory. You can peruse the structure further in our repository, but this is all you need to understand how to test stuff out.

### List of files

#### DOCS
* `DESIGN.pdf`, `DESIGN.txt`, `README.md`, `makefile`

#### EVERYTHING ELSE LOL
* `client.c`, `client.h`
* `func.c`, `func.h`
* `networking.c`, `networking.h`
* `server.c`, `server.h`
* `utils.c`, `utils.h`
* `users/` (see above)

### Parting reminder!!

We have put a great effort into setting up the server-side control AND setting up CSV reading/writing/editing! Sadly, you don't get to see some of those in our final product because we did not manage to integrate them with the wacky, bloated, wobbly control (but which for the most part was functioning...). Some written but unfortunately unused functions are still contained in `func.c`.
