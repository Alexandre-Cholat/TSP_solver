#!/usr/bin/env python3
"""Submit tours and show standings of the TSP Checker API leaderboard
(single file, stdlib only)."""

import argparse
import json
import os
import re
import sys
import urllib.error
import urllib.request
import uuid

DEFAULT_SERVER = "http://10.0.22.151"
TIMEOUT = 10
CHALLENGES = ["CHALL1", "CHALL2", "CHALL3", "CHALL4", "CHALL5"]

# the API lives on the university network: connect directly,
# never through the system HTTP proxy
OPENER = urllib.request.build_opener(urllib.request.ProxyHandler({}))


def unreachable(url):
    print(f"Error: could not reach the TSP Checker API at {url}.")
    print("The API is only reachable from the university network:")
    print("make sure you are connected to the university VPN, then try again.")


def request_json(url, data=None, content_type=None):
    headers = {"Content-Type": content_type} if content_type else {}
    request = urllib.request.Request(url, data=data, method="POST" if data else "GET",
                                     headers=headers)
    try:
        response = OPENER.open(request, timeout=TIMEOUT)
    except urllib.error.HTTPError as e:
        try:
            return e.code, json.loads(e.read())
        except ValueError:
            print(f"Error: unexpected non-JSON response (HTTP {e.code}) "
                  f"from {url}.")
            sys.exit(1)
    except Exception:
        unreachable(url)
        sys.exit(2)
    with response:
        return response.status, json.loads(response.read())


def build_body(challenge, team, solution_path):
    boundary = "----tspcheck" + uuid.uuid4().hex
    body = b""
    for field, value in (("challenge", challenge), ("team", team)):
        head = (f"--{boundary}\r\n"
                f'Content-Disposition: form-data; name="{field}"\r\n\r\n').encode()
        body += head + value.encode() + b"\r\n"
    head = (f"--{boundary}\r\n"
            f'Content-Disposition: form-data; name="solution"; '
            f'filename="{os.path.basename(solution_path)}"\r\n'
            "Content-Type: application/octet-stream\r\n\r\n").encode()
    with open(solution_path, "rb") as f:
        body += head + f.read() + b"\r\n"
    body += f"--{boundary}--\r\n".encode()
    return body, f"multipart/form-data; boundary={boundary}"


def get_data(server):
    status, data = request_json(server.rstrip("/") + "/leaderboard/data")
    if status != 200:
        print(f"Error: {data.get('detail', f'HTTP {status}')}")
        sys.exit(1)
    return data


def cmd_submit(args):
    server = args.server.rstrip("/")
    challenge = args.challenge.upper()
    if challenge not in CHALLENGES:
        print(f"Error: unknown challenge '{args.challenge}'. "
              f"Available challenges: {', '.join(CHALLENGES)}.")
        return 1
    if not re.fullmatch(r"[A-Za-z]", args.team):
        print("Error: team must be a single letter (A-Z).")
        return 1
    if not os.path.isfile(args.solution):
        print(f"Error: file {args.solution} does not exists.")
        return 1

    team = args.team.upper()
    submit_url = server + "/leaderboard/submit"
    body, content_type = build_body(challenge, team, args.solution)
    status, payload = request_json(submit_url, data=body, content_type=content_type)

    if status == 422:
        sys.stdout.write(payload["output"])
        return 1
    if status != 200:
        print(f"Error: {payload.get('detail', f'HTTP {status}')}")
        return 1

    sys.stdout.write(payload["output"])
    if payload["new_team_best"]:
        print(f"New best for Team {team}: {payload['value']}!")
    else:
        data = get_data(server)
        best = next(e["value"] for e in data[challenge]["ENTRIES"]
                    if e["team"] == f"Team {team}")
        print(f"Recorded: {payload['value']} (current best: {best}).")
    return 0


def cmd_show(args):
    data = get_data(args.server)
    for challenge in CHALLENGES:
        print(f"{challenge} ({data[challenge]['DIMENSION']} cities)")
        entries = data[challenge]["ENTRIES"]
        if not entries:
            print("  No submission yet.")
            continue
        for e in entries:
            print(f"  {e['rank']}. {e['team']} \u2014 {e['value']} ({e['date']})")
    return 0


def main():
    parser = argparse.ArgumentParser(
        description="Submit tours and show standings of the TSP Checker API leaderboard."
    )
    sub = parser.add_subparsers(dest="command", required=True)

    p_submit = sub.add_parser("submit", help="submit a tour for a challenge")
    p_submit.add_argument("challenge", help="CHALL1 to CHALL5")
    p_submit.add_argument("solution", help="path to the .sol tour file")
    p_submit.add_argument("team", help="project team letter (A-Z)")
    p_submit.add_argument("--server", default=DEFAULT_SERVER,
                          help="API base URL (default: %(default)s)")
    p_submit.set_defaults(func=cmd_submit)

    p_show = sub.add_parser("show", help="show the current standings")
    p_show.add_argument("--server", default=DEFAULT_SERVER,
                        help="API base URL (default: %(default)s)")
    p_show.set_defaults(func=cmd_show)

    args = parser.parse_args()
    return args.func(args)


if __name__ == "__main__":
    sys.exit(main())
