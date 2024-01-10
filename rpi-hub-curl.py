import RPi.GPIO as GPIO
import time
import requests
import click


def set_website(name, state, silent):
    id = 1 if name == "Male" else 2
    mode = 0 if state else 1
    url = f"http://localhost:3000/t/sensors/{id}"
    if not silent:
        print(url)
    while True:
        try:
            requests.post(url=url, json={"mode": mode})
            break
        except requests.exceptions.RequestException as e:
            print(e)
            time.sleep(2)
    if not silent:
        print(f"{name} Toilet {'Open' if state else 'Closed'}")
    time.sleep(0.1)


@click.command()
@click.option(
    "--mens_room_pin",
    default=21,
    help="Pin number for men's room",
    type=int,
)
@click.option(
    "--ladies_room_pin",
    default=23,
    help="Pin number for ladies room",
    type=int,
)
@click.option(
    "--silent",
    help="Silent mode",
    is_flag=True,
)
def cli(mens_room_pin, ladies_room_pin, silent):
    GPIO.setmode(GPIO.BCM)

    GPIO.setup(
        mens_room_pin, GPIO.IN, pull_up_down=GPIO.PUD_UP
    )  # Men's room Button to GPIO23
    GPIO.setup(
        ladies_room_pin, GPIO.IN, pull_up_down=GPIO.PUD_UP
    )  # Ladies room Button to GPIO21

    try:
        prev_female_state = GPIO.input(ladies_room_pin)
        prev_male_state = GPIO.input(mens_room_pin)
        set_website("Male Init", prev_male_state)
        set_website("Female Init", prev_female_state)

        while True:
            male_state = GPIO.input(23)
            female_state = GPIO.input(21)
            if female_state != prev_female_state:
                if not silent:
                    print("Female bathroom registered: ", female_state)
                set_website("Female", female_state, silent)
                prev_female_state = female_state
            if male_state != prev_male_state:
                if not silent:
                    print("Male bathroom registered: ", male_state)
                set_website("Male", male_state, silent)
                prev_male_state = male_state
    except:
        GPIO.cleanup()


if __name__ == "__main__":
    cli()
