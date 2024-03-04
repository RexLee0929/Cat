document.getElementById('rescueRequestForm').addEventListener('submit', async function(event) {
    event.preventDefault();

    const isLogin = confirm("是否登录以提交申请？选择“确定”登录，选择“取消”则不登录。");

    const onlyValue = 'A' + Date.now().toString();
    const formData = {
        animalInfo: document.querySelector('textarea[name="animalInfo"]').value.trim(),
        animalCount: parseInt(document.querySelector('input[name="animalCount"]').value, 10),
        animalStatus: document.querySelector('textarea[name="animalStatus"]').value.trim(),
        rescueAddress: document.querySelector('textarea[name="rescueAddress"]').value.trim(),
        applicantName: document.querySelector('input[name="applicantName"]').value.trim(),
        applicantGender: document.querySelector('select[name="applicantGender"]').value,
        applicantPhone: document.querySelector('input[name="applicantPhone"]').value.trim(),
        applicantCity: document.querySelector('input[name="applicantCity"]').value.trim(),
        proofImage: document.querySelector('input[name="proofImage"]').value.trim(),
        proofVideo: document.querySelector('input[name="proofVideo"]').value.trim(),
        rescue_only: onlyValue,
        isLogin: isLogin  // 添加到 formData
    };

    // 如果用户选择登录，则添加用户名和密码字段
    if (isLogin) {
        const username = prompt("请输入用户名:");
        const password = prompt("请输入密码:");
        if (!username || !password) {
            alert("用户名和密码不能为空。");
            return;
        }
        formData.username = username;
        formData.password = password;
    }

    try {
        const response = await fetch('/api/submit_rescue_request', {
            method: 'POST',
            headers: {'Content-Type': 'application/json'},
            body: JSON.stringify(formData)
        });

        // 解析JSON响应，无论HTTP状态如何
        const data = await response.json();
        
        if (response.ok) {
            alert(data.message + " 申请单号ID: " + onlyValue);
        } else {
            // 当响应不ok时，我们已经解析了错误信息
            alert("申请提交失败: " + data.error); 
        }
    } catch (error) {
        console.error('Error:', error);
        alert('提交救助请求失败');
    }
});
