/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.js                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 11:53:27 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/29 11:54:21 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

function deleteFile() {
	const filename = document.getElementById("filename").value;

	const xhr = new XMLHttpRequest();
	xhr.open("DELETE", "/delete", true);
	xhr.setRequestHeader("Content-Type", "application/json");
	xhr.send(JSON.stringify({ filename: filename }));
	xhr.onreadystatechange = function () {
		if (xhr.readyState === XMLHttpRequest.DONE) {
			if (xhr.status === 200) {
				alert(xhr.responseText);
			} else {
				alert("An error occurred while deleting the file.");
			}
		}
	};
}
