import requests
import datetime
import time
import numpy as np
import requests
import time
import datetime
import sys
from requests_oauthlib import OAuth2Session


IDS=[
	("u-s4t2ud-4c150f08adb6bfe1fcec95f1e89c29c57466fb64a84190e8e848547974a1682e", "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")
]


def get_access_token():
	base_url = "https://api.intra.42.fr"
	token_url = f"{base_url}/oauth/token"
	data = {
		"grant_type": "client_credentials",
		"client_id": IDS[0][0],
		"client_secret": IDS[0][1]
	}

	# Request an access token using the client credentials grant type
	response = requests.post(token_url, data=data)
	response_json = response.json()
	access_token = response_json["access_token"]
	return access_token

def get_campus_users(access_token, params):
	base_url = "https://api.intra.42.fr"
	endpoint = "/v2/campus"

	# Set the Authorization header with the access token
	headers = {
		"Authorization": f"Bearer {access_token}"
	}

	# Make the GET request to the /v2/campus_users endpoint with the params
	response = requests.get(f"{base_url}{endpoint}", headers=headers, params=params)
	campus_users = response.json()

	return campus_users


def get_42mulhouse_users(access_token, filters={}):
	total_users = []
	users = [1]
	params = {
		"filter[primary_campus_id]": 12,
		"filter[kind]": "student",
		"page[size]": 100
	}
	headers = {
		"Authorization": f"Bearer {access_token}"
	}
	params.update(filters)
	page = 1
	while (len(users) != 0):
		params["page[number]"] = page
		x = requests.get(f"https://api.intra.42.fr/v2/users", headers=headers, params=params,)
		if (x.status_code != 200):
			raise Exception("Bad status code")
		users = x.json()
		for e in users:
			if (e["active?"]):
				total_users.append(e)
		page += 1
	return total_users

def get_full_user_info(access_token, user_id):
	headers = {
		"Authorization": f"Bearer {access_token}"
	}
	x=requests.get(f"https://api.intra.42.fr/v2/users/{user_id}", headers=headers)
	return x.json()

if __name__ == "__main__":
	params = {
		"user_id": "12"
	}
	filters = {"filter[login]":"fluchten"}
	access_token = get_access_token()
	total_users = get_42mulhouse_users(access_token, filters)
	if (len(total_users) == 0):
		print("No users found")
		exit(0)
	print(total_users[0]["id"])
	user = get_full_user_info(access_token, total_users[0]["id"])
	print(user)
	campus_users = get_campus_users(access_token, params)
	# print(campus_users)