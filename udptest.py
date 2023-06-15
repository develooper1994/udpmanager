# This Python file uses the following encoding: utf-8
#! /usr/bin/python3

import socket
import random
import string

def generate_data(size):
    return ''.join(random.choices(string.ascii_uppercase +
string.ascii_lowercase + string.digits, k=size))

def send_data(begin, end):
    host = 'localhost'  # Replace with the IP address or hostname of the
receiving machine port = 9008  # Replace with the desired port number

    data = f"{begin}#{generate_data(60000)}{end}"

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    try:
        sock.sendto(data.encode(), (host, port))
        print("Data sent successfully!")
    except socket.error as e:
        print(f"Error while sending data: {str(e)}")
    finally:
        sock.close()

if __name__ == "__main__":
    # Call the function with begin and end values
    send_data("HATLIST#158", "OK")
