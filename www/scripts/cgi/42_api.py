import requests
import datetime
import time
import numpy as np
import requests
import time
import datetime
import sys
import os
from urllib.parse import parse_qs
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

	response = requests.post(token_url, data=data)
	response_json = response.json()
	access_token = response_json["access_token"]
	return access_token

def get_19_campus_user(access_token, filters={}):
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

def find_corrected_users(json_data, corrector_id):
	corrected_users = {}    
	for evaluation in json_data:
		corrector = evaluation.get('corrector', {}).get('id')
		if corrector == corrector_id:
			for corrected in evaluation.get('correcteds', []):
				corrected_id = corrected.get('login')
				if corrected_id in corrected_users:
					corrected_users[corrected_id] += 1
				else:
					corrected_users[corrected_id] = 1
					
	return corrected_users

def get_all_correction(user_id):
	headers = {
		"Authorization": f"Bearer {access_token}"
	}
	page = 1
	pgdata = [None]
	total_data = []
	while (len(pgdata) != 0):
		x=requests.get(f"https://api.intra.42.fr/v2/users/{user_id}/scale_teams/", headers=headers, params={"page[size]":100, "page[number]":page})
		pgdata = x.json()
		total_data += pgdata
		page += 1
	return total_data

if __name__ == "__main__":
	query_string = os.environ.get('QUERY_STRING', '')
	query_params = parse_qs(query_string)
	login = query_params.get('login', [''])[0]

	if not login:
		sys.stdout.write("No users found.")
		sys.exit(0)

	filters = {"filter[login]": login}
	access_token = get_access_token()
	total_users = get_19_campus_user(access_token, filters)
	if len(total_users) == 0:
		sys.stdout.write("No users found.")
		sys.exit(0)
	user_id = total_users[0]["id"]
	data_json = get_all_correction(user_id)
	corrected_users = find_corrected_users(data_json, user_id)
	sorted_data = sorted(corrected_users.items(), key=lambda x: x[1], reverse=True)

	html_content = "<!DOCTYPE html>\r\n"
	html_content += "<html lang=\"en\">\r\n"
	html_content += "<head>\r\n"
	html_content += "\t<meta charset=\"UTF-8\">\r\n"
	html_content += "\t<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\r\n"
	html_content += "\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n"
	html_content += "\t<link rel=\"stylesheet\" href=\"./style/cgi/42api.css\">\r\n"
	html_content += "\t<title>GGI - 42 API</title>\r\n"
	html_content += "</head>\r\n"
	html_content += "<body>\r\n"
	html_content += "\t<table>\r\n"
	html_content += "\t\t<tr>\r\n"
	html_content += "\t\t\t<th>Position</th>\r\n"
	html_content += "\t\t\t<th>Login</th>\r\n"
	html_content += "\t\t\t<th>Number of corrections</th>\r\n"
	html_content += "\t\t\t<th>Link intra</th>\r\n"
	html_content += "\t\t</tr>\r\n"
	for index, (person, corrections) in enumerate(sorted_data, start=1):
		intra_link = f"https://profile.intra.42.fr/users/{person}"
		html_content += "\t\t<tr>\r\n"
		html_content += f"\t\t\t<td>{index}</td>\r\n"
		html_content += f"\t\t\t<td>{person}</td>\r\n"
		html_content += f"\t\t\t<td>{corrections}</td>\r\n"
		html_content += f"\t\t\t<td><a href=\"{intra_link}\" target=\"_blank\">Link intra</a></td>\r\n"
		html_content += "\t\t</tr>\r\n"
	html_content += "\t</table>\r\n"
	html_content += "</body>\r\n"
	html_content += "</html>"


	sys.stdout.write(html_content)
