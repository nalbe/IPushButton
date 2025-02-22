
Explanation of the update() function.
Start: Begin the update() function.
- Is enabled?: Check if the button is enabled. If not, exit.
- Is debounce delay active?: Check if the button is within the debounce delay period.
    - If yes, check if the button is pushed:
        - If pushed, set state to DELAY and call onDelayFn().
        - If not pushed, set state to IDLE and call onIdleFn().
- Is button not pushed and not pressed?: Check if the button is idle.
    - If yes, set state to IDLE and call onIdleFn().
- Is button just pushed?: Check if the button has just been pushed.
    - If yes, check if it’s a "mash" (rapid click):
        - If it’s a mash, increment mashCount() and set state to PUSH | MASH.
        - If not, reset mashCount() and set state to PUSH.
    - Call onPushFn().
- Is button released?: Check if the button has been released.
    - If yes, set state to RELEASE and call onReleaseFn().
- Is repeat delay active?: Check if the button is within the repeat delay period.
    - If no, set state to HOLD and call onHoldFn().
    - If yes, set state to DELAY and call onDelayFn().
End: Exit the function.


