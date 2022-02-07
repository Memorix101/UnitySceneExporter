using System.Collections;
using System.Collections.Generic;
using System.IO;
using System;
using System.Text;
using UnityEngine;
using UnityEditor;
using UnityEngine.SceneManagement;

public class CustomExporter : Editor
{
    [MenuItem("Scene Exporter/Import")]
    private static void ImportDeg()
    {
        Debug.Log("Import");

        GameObject[] allObjects = SceneManager.GetActiveScene().GetRootGameObjects(); // UnityEngine.Object.FindObjectsOfType<GameObject>();
        foreach (var item in allObjects)
        {
            DestroyImmediate(item);
        }

        // https://docs.unity3d.com/ScriptReference/EditorUtility.OpenFilePanel.html
        string path = EditorUtility.OpenFilePanel("Load scene file", "", "lvl");
        if (path.Length != 0)
        {
            string[] lines = File.ReadAllLines(path);
            bool isLVL = false;
            foreach (string line in lines)
            {
                //Console.WriteLine("\t" + line);

                if (line == "lvl1")
                {
                    isLVL = true;
                    continue;
                }

                if (line[0] == '#')
                {
                    continue;
                }

                if (isLVL)
                {
                    // parsePos
                    string model, name, tag = null; //0,1,2
                    float x, y, z = 0f; //3,4,5
                    float rx, ry, rz, rw = 0f; //6,7,8,9
                    float sx, sy, sz = 0f; //10,11,12
                    string[] tmp = line.Split(',');

                    if (tmp.Length > 12)
                    {
                        model = tmp[0];
                        name = tmp[1];
                        tag = tmp[2];
                        x = float.Parse(tmp[3], System.Globalization.CultureInfo.InvariantCulture);
                        y = float.Parse(tmp[4], System.Globalization.CultureInfo.InvariantCulture);
                        z = float.Parse(tmp[5], System.Globalization.CultureInfo.InvariantCulture);
                        rx = float.Parse(tmp[6], System.Globalization.CultureInfo.InvariantCulture);
                        ry = float.Parse(tmp[7], System.Globalization.CultureInfo.InvariantCulture);
                        rz = float.Parse(tmp[8], System.Globalization.CultureInfo.InvariantCulture);
                        rw = float.Parse(tmp[9], System.Globalization.CultureInfo.InvariantCulture);
                        sx = float.Parse(tmp[10], System.Globalization.CultureInfo.InvariantCulture);
                        sy = float.Parse(tmp[11], System.Globalization.CultureInfo.InvariantCulture);
                        sz = float.Parse(tmp[12], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    else
                    {
                        model = tmp[0];
                        name = tmp[1];
                        tag = tmp[2];
                        x = float.Parse(tmp[3], System.Globalization.CultureInfo.InvariantCulture);
                        y = float.Parse(tmp[4], System.Globalization.CultureInfo.InvariantCulture);
                        z = float.Parse(tmp[5], System.Globalization.CultureInfo.InvariantCulture);
                        rx = float.Parse(tmp[6], System.Globalization.CultureInfo.InvariantCulture);
                        ry = float.Parse(tmp[7], System.Globalization.CultureInfo.InvariantCulture);
                        rz = float.Parse(tmp[8], System.Globalization.CultureInfo.InvariantCulture);
                        sx = float.Parse(tmp[9], System.Globalization.CultureInfo.InvariantCulture);
                        sy = float.Parse(tmp[10], System.Globalization.CultureInfo.InvariantCulture);
                        sz = float.Parse(tmp[11], System.Globalization.CultureInfo.InvariantCulture);
                    }

                    /*if (model == "null")
                        continue;*/

                    // searching for requested asset in project directory
                    string[] fileEntries = Directory.GetFiles(Application.dataPath);
                    // go through subdirectories
                    string[] subdirectoryEntries = Directory.GetDirectories(Application.dataPath);
                    foreach (string subdirectory in subdirectoryEntries)
                    {
                        string[] subfileEntries = Directory.GetFiles($"{subdirectory}");

                        //concatenate both arrays
                        string[] newfileEntries = new string[fileEntries.Length + subfileEntries.Length];
                        fileEntries.CopyTo(newfileEntries, 0);
                        subfileEntries.CopyTo(newfileEntries, fileEntries.Length);
                        Array.Clear(fileEntries, 0, fileEntries.Length);
                        Array.Resize(ref fileEntries, newfileEntries.Length);
                        newfileEntries.CopyTo(fileEntries, 0);
                    }

                    ArrayList arrayList = new ArrayList();
                    foreach (string s in fileEntries)
                    {
                        arrayList.Add(s.Remove(0, Application.dataPath.Length));
                    }

                    string modelName = string.Empty;
                    foreach (string s in arrayList)
                    {
                        string[] sp = s.Split("\\");
                        if (model == sp[sp.Length - 1])
                        {
                            modelName = s;
                        }
                    }

                    var asset = AssetDatabase.LoadAssetAtPath($"Assets{modelName}", typeof(GameObject));


                    GameObject go = new GameObject(name);
                    // Set the path as within the Assets folder,
                    // and name it as the GameObject's name with the .Prefab format
                    string localPath = "Assets/" + go.name + ".prefab";
                    // Make sure the file name is unique, in case an existing Prefab has the same name.
                    localPath = AssetDatabase.GenerateUniqueAssetPath(localPath);
                    // Create the new Prefab.

                    if (tag == "MainCamera")
                    {
                        go.AddComponent<Camera>();
                        go.AddComponent<Camera>();
                        go.AddComponent<AudioListener>();
                        go.tag = "MainCamera";
                        go.transform.position = new Vector3(x, y, z);
                        if (tmp.Length > 12) go.transform.localRotation = new Quaternion(rx, ry, rz, rw); else go.transform.eulerAngles = new Vector3(rx, ry, rz); 
                    }
                    else if (model != "null")
                    {
                        GameObject c = PrefabUtility.InstantiatePrefab(asset) as GameObject;
                        DestroyImmediate(go); // Instantiate creates a clone and so we delete the original and rename the clone to vanish the traces
                        c.name = name;
                        c.tag = tag;
                        c.transform.position = new Vector3(x, y, z);
                        if (tmp.Length > 12) c.transform.localRotation = new Quaternion(rx, ry, rz, rw); else c.transform.eulerAngles = new Vector3(rx, ry, rz);
                        c.transform.localScale = new Vector3(sx, sy, sz);
                    }

                    Debug.Log("OUTPUT:");
                    Debug.Log($" model: {model}\n name:  {name}\n tag: {tag}");
                    Debug.Log($" pos:  {x} {y} {z}");
                    if (tmp.Length > 12) Debug.Log($" rot: {rx} {ry} {rz} {rw}"); else Debug.Log($" rot: {rx} {ry} {rz}");
                    Debug.Log($" scale: {sx} {sy} {sz} ");
                }
            }
        }
    }

    [MenuItem("Scene Exporter/Export (Degrees)")]
    private static void ExportDeg()
    {
        Debug.Log("Export (Degrees)");

        string outData = string.Empty;
        string head = "# rotation in degrees";
        string magicNumber = "lvl1";

        // https://answers.unity.com/questions/27729/finding-the-root-gameobjects-in-the-scene-.html
        GameObject[] allObjects = SceneManager.GetActiveScene().GetRootGameObjects(); // UnityEngine.Object.FindObjectsOfType<GameObject>();
        foreach (GameObject go in allObjects)
        {
            //Debug.Log(go.name);
            string _path = PrefabUtility.GetPrefabAssetPathOfNearestInstanceRoot(go);
            Debug.Log(_path);
            string[] _name = _path.Split('/');
            if (_path == string.Empty) { outData += "null" + ","; } else { outData += _name[_name.Length - 1] + ","; }
            outData += go.name + ",";
            outData += go.tag + ",";
            outData += go.transform.position.x + ",";
            outData += go.transform.position.y + ",";
            outData += go.transform.position.z + ",";
            outData += go.transform.localEulerAngles.x + ",";
            outData += go.transform.localEulerAngles.y + ",";
            outData += go.transform.localEulerAngles.z + ",";
            outData += go.transform.localScale.x + ",";
            outData += go.transform.localScale.y + ",";
            outData += go.transform.localScale.z + "\n";
        }

        //https://docs.unity3d.com/ScriptReference/EditorUtility.SaveFilePanel.html
        var path = EditorUtility.SaveFilePanel(
           "Save scene file",
           "",
           SceneManager.GetActiveScene().name + ".lvl",
           "lvl");

        if (path.Length != 0)
        {
            var data = Encoding.ASCII.GetBytes($"{magicNumber}\n{head}\n{outData}");
            File.WriteAllBytes(path, data);
        }
    }

    [MenuItem("Scene Exporter/Export (Quaternion)")]
    private static void ExportQuat()
    {
        Debug.Log("Export (Quaternion)");

        string outData = string.Empty;
        string head = "# rotation in quaternion";
        string magicNumber = "lvl1";

        // https://answers.unity.com/questions/27729/finding-the-root-gameobjects-in-the-scene-.html
        GameObject[] allObjects = SceneManager.GetActiveScene().GetRootGameObjects(); // UnityEngine.Object.FindObjectsOfType<GameObject>();
        foreach (GameObject go in allObjects)
        {
            //Debug.Log(go.name);
            string _path = PrefabUtility.GetPrefabAssetPathOfNearestInstanceRoot(go);
            Debug.Log(_path);
            string[] _name = _path.Split('/');
            if (_path == string.Empty) { outData += "null" + ","; } else { outData += _name[_name.Length - 1] + ","; }
            outData += go.name + ",";
            outData += go.tag + ",";
            outData += go.transform.position.x + ",";
            outData += go.transform.position.y + ",";
            outData += go.transform.position.z + ",";
            outData += go.transform.localRotation.x + ",";
            outData += go.transform.localRotation.y + ",";
            outData += go.transform.localRotation.z + ",";
            outData += go.transform.localRotation.w + ",";
            outData += go.transform.localScale.x + ",";
            outData += go.transform.localScale.y + ",";
            outData += go.transform.localScale.z + "\n";
        }

        //https://docs.unity3d.com/ScriptReference/EditorUtility.SaveFilePanel.html
        var path = EditorUtility.SaveFilePanel(
           "Save scene file",
           "",
           SceneManager.GetActiveScene().name + ".lvl",
           "lvl");

        if (path.Length != 0)
        {
            var data = Encoding.ASCII.GetBytes($"{magicNumber}\n{head}\n{outData}");
            File.WriteAllBytes(path, data);
        }
    }
}