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
